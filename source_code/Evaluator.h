#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include "Memory.h"
#include "HashTable.h"

class Evaluator{
    public:
        Evaluator(Memory& memory, HashTable& hashtable);

        // 1. Eval() : reads the root of a parse tree and returns the result of evaluation
        // 1) return negative int or 0 : hash value(result is a symbol, number, or #t, #f)
        // 2) return positive int : node index(result is a list)
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


        // 3. IsNumber() : check if the result(hash value) is a number 
        bool IsNumber(int value) const;

        // 4. GetVal() : get the result(hash value) and return the symbol(number) in double
        double GetVal(int value) const;

        // 5. MakeNumber() : change the calculation result(double) into a symbol and insert, then return its hash value
        int MakeNumber(double value);

        // 6. UserFunc() : helper function for calling user defined functions 
        // restoring the original value(use ElemStack)
        // funcExp : name(symbol) of the function
        // argRoot : node index that points to the function argument
        int UserFunc(int funcExp, int argRoot);

        // 7. PrintList() : used in public memeber function PrintResult() to recursively print a list
        void PrintList(int root, bool startList);
};

#endif