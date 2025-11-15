#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "Memory.h"
#include "HashTable.h"

class Evaluator{
    public:
        Evaluator(Memory& memory, HashTable& hashtable);

        // 1. Eval() : reads the root of a parse tree and returns the result of evaluation
        int Eval(int root);

        // 2. PrintResult() : prints the result of evaluation
        void PrintResult(int result, bool startList);

    private:
        Memory& m;
        HashTable& htab;

        // hash value of built-in symbols : for convenience when comparing with token index
        int PLUS; // "+"
        int MINUS; // "-"
        int MULT; // "*"
        int DIV; // "/"

        int ISNUMBER; // "number?"
        int ISNULL; // "null?"

        int CONS; // "cons"
        int CAR; // "car"
        int CDR; // "cdr"
        int COND; // "cond"
        int QUOTE; // "quote"
        int DEFINE; // "define"
        int LAMBDA; // "lambda"
        int ELSE; // "else"

        int TRUE_SYM; // "#t"
        int FALSE_SYM; // "#f"

        // pseudocode에는 없지만 구현해 봤습니다
        int EQ; // "="
        int RG; // "<"
        int LG; // ">"

        // 3. UserFunc() : helper function for calling user defined functions 
        // restoring the original value(use ElemStack)
        // funcExp : name(symbol) of the function
        // argRoot : node index that points to the function argument
        int UserFunc(int funcExp, int argRoot);

        // 4. IsNumber() : check if the result(hash value) is a number 
        bool IsNumber(int value) const;

        // 5. GetVal() : get the result(hash value) and return the symbol(number) in double
        double GetVal(int value) const;

        // 6. MakeNumber() : change the calculation result(double) into a symbol and insert, then return its hash value
        int MakeNumber(double value);

        
};

#endif