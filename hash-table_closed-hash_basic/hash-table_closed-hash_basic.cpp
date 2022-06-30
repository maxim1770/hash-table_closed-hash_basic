#include <iostream>

using namespace std;

typedef int T;

class AbstractKey
{
public:
    virtual int getHashCode() = 0;
    virtual bool compare(AbstractKey* key) = 0;
};

class Person : public AbstractKey
{
private:
    string name;
    int hash_mul;

public:
    Person(string name)
    {
        this->name = name;
        this->hash_mul = 23;
    }

    int getHashCode()
    {
        int hashcode = 0;

        for (int i = 0; i < this->name.size(); i++)
        {
            hashcode = hashcode * this->hash_mul + (int)this->name[i];
        }

        return hashcode;
    }

    bool compare(AbstractKey* key)
    {
        Person* otherPerson = (Person*)key;
        return this->name == otherPerson->name;
    }
};

enum ElementStatus
{
    EMPTY,
    FILLED,
    DELETED
};

struct Element
{
    ElementStatus status;
    AbstractKey* key;
    T data;

    Element(AbstractKey* key, T data)
    {
        this->key = key;
        this->data = data;
        this->status = FILLED;
    }
};

struct HashTable
{
private:
    int C1 = 1;

    Element** hash_table;
    int hash_table_size;

    int getHashCode(AbstractKey* key, int i)
    {
        int hashcode = key->getHashCode();
        return (hashcode + C1 * i) % this->hash_table_size;
    }

    bool canInsert(Element* el)
    {
        return el == nullptr || el->status == EMPTY || el->status == DELETED;
    }

    bool isEmpty(Element* el)
    {
        return el == nullptr || el->status == EMPTY;
    }

    bool isDeleted(Element* el)
    {
        return el != nullptr && el->status == DELETED;
    }

public:
    HashTable(int size)
    {
        this->hash_table = new Element * [size];
        this->hash_table_size = size;

        for (int i = 0; i < size; i++)
        {
            this->hash_table[i] = nullptr;
        }
    }

    bool insert(AbstractKey* key, T value)
    {
        // reserch
        for (int i = 0; i < this->hash_table_size; i++)
        {
            int hashcode = this->getHashCode(key, i);

            Element* el = this->hash_table[hashcode];

            if (this->canInsert(el))
            {
                if (el != nullptr)
                {
                    el->key = key;
                    el->data = value;
                    el->status = FILLED;
                }
                else
                {
                    Element* newEl = new Element(key, value);
                    this->hash_table[hashcode] = newEl;
                }
                return true;
            }
        }

        return false;
    }

    bool remove(AbstractKey* key)
    {

        for (int i = 0; i < this->hash_table_size; i++)
        {
            int hashcode = this->getHashCode(key, i);
            Element* el = this->hash_table[hashcode];

            if (this->isEmpty(el) || this->isDeleted(el) && key->compare(el->key))
            {
                return false;
            }

            if (key->compare(el->key))
            {
                el->status = DELETED;
                return true;
            }
        }

        return false;
    }

    T find(AbstractKey* key)
    {

        for (int i = 0; i < this->hash_table_size; i++)
        {
            int hashcode = this->getHashCode(key, i);
            Element* el = this->hash_table[hashcode];

            if (this->isEmpty(el) || this->isDeleted(el) && key->compare(el->key))
            {
                return -1;
            }

            if (key->compare(el->key))
                return el->data;
        }

        return -1;
    }
};

int main()
{
    HashTable hash_table(24);

    AbstractKey* key1 = new Person("Ivan");
    AbstractKey* key2 = new Person("Petya");

    hash_table.insert(key1, 23);
    hash_table.insert(key2, 48);

    cout << hash_table.find(key1) << endl;
    cout << hash_table.find(key2) << endl;

    hash_table.remove(key2);

    cout << hash_table.find(key2) << endl;


    return 0;
}
