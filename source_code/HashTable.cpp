#include "HashTable.h"
#include <iostream>

// constructor : initializes the HashTable
HashTable::HashTable(int size)
{
    capacity = size; // fixed size

    Table = new Element[capacity]; // dynamic allocation

    Table[0].hashValue = 0;
    Table[0].symbol = ""; // Null String for index 0
    Table[0].linkOfValue = 0;

    for(int i = 1; i < capacity; i++)
    {
        Table[i].hashValue = -i; // initialize as negative index
        Table[i].symbol = ""; // initially empty
        Table[i].linkOfValue = 0; // initially all links are set to 0
    }
}

// destructor
HashTable::~HashTable()
{
    delete[] Table;
}

// 1. Hash function(open addressing, linear probing)
int HashTable::Hash(std::string key, int idx)
{
    if(key.empty()) // null string
    {
        return 0;
    }
    else // not null string
    {
        int length = key.length();
        int keySum = 0;

        // linear probing by index i
        for(int i = 0; i < length; i++)
        {
            keySum = keySum + key[i];
        }

        // use negative index for hash value (0 can appear but symbol in index 0 is always null string)
        int mod = (keySum + idx) % capacity; 

        return -mod; 
    }
}

// 2. Find Symbol corresponding to HashValue
std::string HashTable::FindSymbol(int j)
{
    return Table[-j].symbol;
    // if return is "" and j is not 0 index : j is an empty slot
    // if return is not "" or j is 0 index : j is occupied
}

// 3. Hash Insert : insert a new symbol into the hash table
void HashTable::HashInsert(std::string key)
{
    int i = 0;
    while(i < capacity)
    {
        int j = Hash(key, i); // j is 0 or negative

        std::string s = FindSymbol(j);
        // if s is "" and j is not 0 index : j is an empty slot
        // if s is not "" or j is 0 index : j is occupied

        if(s == "" && j != 0) // empty slot
        {
            Table[-j].symbol = key;
            break;
        }
        else if (s == key) // same as the key : do not insert again
        {
            return;
        }
        else // Collision(j is occupied) : linear probing 
        {
            i++;
        }
    }

    if(i == capacity)
    {
        throw std::runtime_error{"Hash Table Overflow"};
    }
}

// 4. Hash Search : get hash value of input string
// returns hash value if found
// returns -capacity if not found 
// -capacity doesn't exist in the hash value , but is used as a indicator to tell Read() function to insert new symbol
// not to be confused when the searchKey is a null string
int HashTable::GetHashValue(std::string searchKey)
{
    int i = 0;
    while(i < capacity)
    {
        int j = Hash(searchKey, i);

        if(Table[-j].symbol == searchKey) // 1. found the searchKey
        {
            return j;
        }
        else if(Table[-j].symbol == "" && j != 0)
        {
            return -capacity; // 2. fail to find : don't have to search anymore
        }
        else
        {
            i++;
        }
    }

    return -i; // 3. fail to find : (i == capacity)
}


// 5. BuiltIn : Insert built in symbols first into the hash table before getting the input
void HashTable::BuiltIn() 
{
    // index 0 is already processed in constructor

    // built in symbols
    HashInsert("(");
    HashInsert(")");

    
    // HashInsert("+");
    // HashInsert("-");
    // HashInsert("*");
    // HashInsert("/");
    // HashInsert("=");
    // HashInsert("<");
    // HashInsert(">");
    // HashInsert("car");
    // HashInsert("cdr");
    // HashInsert("cons");
    // HashInsert("null?");
    // HashInsert("display");
    // HashInsert("error");
}