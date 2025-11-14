#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <string>

struct Element{
    int hashValue;
    std::string symbol;
    int linkOfValue;
};

class HashTable{
    public:
        // constructor : initializes the HashTable
        HashTable(int size);
        // destructor
        ~HashTable();

        // 1. Hash function(open addressing, linear probing)
        int Hash(std::string key, int idx); 

        // 2. Find Symbol corresponding to HashValue
        std::string FindSymbol(int j);

        // 3. Hash Insert : insert a new symbol into the hash table
        void HashInsert(std::string key);

        // 4. Hash Search : get hash value of input string
        int GetHashValue(std::string searchKey);

        // 5. BuiltIn : Insert built in symbols first into the hash table before getting the input
        void BuiltIn();

        // 6. Getters, Setters
        Element& getElem(int idx) {return Table[idx];}
        int getCapacity() {return capacity;}

        // 7. Get LinkOfValue from hash value
        int getLinkOfValue(int hashValue) const
        {
            if(hashValue == 0) return 0; // empty list ()
            return Table[-hashValue].linkOfValue;
        }

        // 8. Set LinkOfValue of hash value as a new link
        void setLinkOfValue(int hashValue, int link)
        {
            if(hashValue == 0) return;
            Table[-hashValue].linkOfValue = link;
        }

    private:
        Element* Table; 
        int capacity;
};

#endif