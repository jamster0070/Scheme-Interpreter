// #### SCHEME INTERPRETER 2020-16468 이준하 #### //
// compile : 
// g++ -std=c++20 -o scheme_interpreter main.cpp Tokenizer.cpp Parser.cpp Memory.cpp HashTable.cpp Preprocessor.cpp ElemStack.cpp Evaluator.cpp

#include <iostream>
#include <string>
#include "Parser.h"
#include "Preprocessor.h"
#include "Evaluator.h"

int main() 
{
    const int size = 101; // fixed size

    // Memory and Hash Table are initialized only once
    Memory memory(size);
    HashTable hashtable(size);

    // Insert built in symbols first
    hashtable.BuiltIn();

    while(true)
    {
        std::string cmd;
        std::cout << "> "; // user input for scheme code
        std::getline(std::cin, cmd);
        if(cmd.empty()) {break;}

        // #### 1. Preprocess ####

        Tokenizer tmp(cmd);
        std::string newCmd;
        preprocess(tmp, newCmd);

        std::cout << "[Preprocessed] " << newCmd << std::endl;
    
        // #### 2. Read ####
        // create objects
        Tokenizer tokenizer(newCmd);
        Parser parser(tokenizer, memory, hashtable);

        // Read()
        int parseTreeRoot = parser.Read();

        // // Print Output start
        // std::cout << "] Free list's root = " << memory.getFreeRoot() << std::endl;;
        // std::cout << "Parse tree's root = " << parseTreeRoot << std::endl;
        // std::cout << std::endl;

        // // 1. Node Array
        // std::cout << "Node array = \n";
        // // Header
        // std::cout << "Node Id\t\t| Left\t\t| Right\n";
        // std::cout << "----------------#---------------#---------------\n";

        // for(int i = 0; i  < memory.getCapacity(); i++)
        // {
        //     const Node& node = memory.getNode(i);
        //     std::cout << i << "\t\t| " << node.lchild << "\t\t| " << node.rchild << std::endl;
        // }
        // std::cout << std::endl;

        // // 2. Hash Table
        // std::cout << "Hash table = \n";
        // // Header
        // std::cout << "Hash Value\t| Symbol\t| Link\n";
        // std::cout << "----------------#---------------#---------------\n";

        // for(int i = 0; i < hashtable.getCapacity(); i++)
        // {
        //     const Element& elem = hashtable.getElem(i);
        //     if(elem.symbol != "" || i == 0) // non-empty elements
        //     {
        //         std::cout << elem.hashValue << "\t\t| ";

        //         if(i == 0)
        //         {
        //             std::cout << "()";
        //             for(int k = 0; k < 12; k++) 
        //             {
        //                 std::cout << " ";
        //             }
        //         }
        //         else
        //         {
        //             std::cout << elem.symbol;

        //             int padd = 14 - elem.symbol.length();
        //             for(int k = 0; k < padd; k++)
        //             {
        //                 std::cout << " ";
        //             }
        //         }
        //         std::cout << "| " << elem.linkOfValue << std::endl;
        //     }    
        // }
        // std::cout << std::endl;

        // // Print()
        // parser.Print(parseTreeRoot, true);
        // std::cout << std::endl;

        // // return parse tree to free list
        // memory.returnTree(parseTreeRoot);

        // #### 3. Evaluation ####
        Evaluator evaluator(memory, hashtable);
        int result = evaluator.Eval(parseTreeRoot);

        // #### 4. PrintResult ####
        std::cout << "[Result] ";
        evaluator.PrintResult(result, true);
        std:: cout << std::endl;
    }
    return 0;
}
