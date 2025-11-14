#ifndef ELEMSTACK_HPP
#define ELEMSTACK_HPP

#include "HashTable.h"

// "Element" data type of HashTable is used in the stack
class ElemStack{
    public:
        ElemStack(int stackCapacity);
        ~ElemStack();

        bool IsEmpty() const;
        void ChangeSize(int newCapacity);
        void Push(const Element& x);
        void Pop();
        Element& Top() const;

    private:
        Element* S;
        int top;
        int capacity;
};

#endif