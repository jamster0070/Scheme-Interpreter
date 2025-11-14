#ifndef STACK_HPP
#define STACK_HPP

#include "HashTable.h"

// "Element" data type of HashTable is used in the stack
class Stack{
    public:
        Stack(int stackCapacity);
        ~Stack();

        bool IsEmpty() const;
        void Push(const Element& x);
        void Pop();
        Element& Top() const;

    private:
        Element* stack;
        int top;
        int capacity;
};

#endif