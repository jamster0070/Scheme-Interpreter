#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "Tokenizer.h"
#include "Memory.h"
#include "HashTable.h"

class Parser{
    public:
        // constructor
        Parser(Tokenizer& tokenizer, Memory& memory, HashTable& hashtable);
        // destructor
        ~Parser();

        // 1. Read() : read the input and create a parse tree
        int Read();

        // 2. Print() : read the parse tree and print the original input
        void Print(int root, bool startList);

        // 3. PrintOutput : print the desired output
        void PrintOutput();


    private:
        Tokenizer& t;
        Memory& m;
        HashTable& htab;
};

#endif