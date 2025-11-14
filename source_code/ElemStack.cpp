#include "ElemStack.h"
#include <iostream>

// constructor
ElemStack::ElemStack(int stackCapacity): capacity(stackCapacity)
{
    if(stackCapacity <= 0) stackCapacity = 1;
    capacity = stackCapacity;
    S = new Element[capacity];
    top = -1; // empty stack
} 

// destructor
ElemStack::~ElemStack()
{
    delete[] S;
}

bool ElemStack::IsEmpty() const
{
    return (top == -1);
}

void ElemStack::ChangeSize(int newCapacity)
{
    if(newCapacity <= capacity) return;

    Element* newS = new Element[newCapacity];

    // copy existing elements
    for(int i = 0; i <= top; i++) newS[i] = S[i];

    delete[] S;
    S = newS;
    capacity = newCapacity;
}


void ElemStack::Push(const Element& x)
{
    if(top == capacity - 1) // full
    {
        ChangeSize(2 * capacity);
    }

    top++;
    S[top] = x; // copy of Element (hashValue, symbol, linkOfValue)
}

void ElemStack::Pop()
{
    if(IsEmpty()) throw std::runtime_error("[Pop] Stack is empty. Cannot delete");
    top--;
}

Element& ElemStack::Top() const
{
    if(IsEmpty()) throw std::runtime_error("[Top] Stack is empty");
    return S[top];
}