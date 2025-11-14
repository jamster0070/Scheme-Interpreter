#include "Memory.h"
#include <iostream>
 
// constructor
Memory::Memory(int size) 
{
    capacity = size; // fixed as 101

    nodeArr = new Node[capacity]; // dynamic allocation

    // index 0 for empty list ()
    nodeArr[0].rchild = 0;
    nodeArr[0].lchild = 0;

    // freeRoot starts from index 1
    freeRoot = 1;

    for(int i = 1; i < capacity - 1; i++)
    {
        nodeArr[i].rchild = i + 1; // ith node's tail points at (i+1)th node
        nodeArr[i].lchild = 0; // initialize lchild as 0 -> change to hash value or index number at Read()
    }

    // index capacity-1 (last)
    nodeArr[capacity - 1].rchild = 0; // last node's tail points at 0
    nodeArr[capacity - 1].lchild = 0;
}

// destructor
Memory::~Memory()
{
    delete[] nodeArr;
}

// 1. Allocation to the parse tree
int Memory::allocNode()
{
    if(freeRoot == 0)
    {
        throw std::runtime_error("Node Array Overflow");
    }
    int idx = freeRoot;

    // freeRoot moves to next node(current node's right child)
    freeRoot = nodeArr[idx].rchild; 

    // allocate to parse tree -> get ready for Read()
    nodeArr[idx].rchild = 0;
    nodeArr[idx].lchild = 0;

    // return old freeRoot's index
    return idx;
}

// 2. Return to the memory
void Memory::returnOne(int idx)
{
    nodeArr[idx].rchild = freeRoot;
    nodeArr[idx].lchild = 0;

    freeRoot = idx;
}

void Memory::returnTree(int root)
{
    // end condition : rchild == 0(stop) or lchild < 0(stop)
    if(root <= 0) {return;}

    // if node's rchild > 0 : there are more leaf nodes remaining on the right -> recursion on the right(root = rchild)
    if(nodeArr[root].rchild > 0) {returnTree(nodeArr[root].rchild);}

    // if node's lchild > 0 : there are more leaf nodes remaining on the left -> recursion on the left(root = lchild)
    if(nodeArr[root].lchild > 0) {returnTree(nodeArr[root].lchild);}

    // free the root node itself
    returnOne(root); 
}


