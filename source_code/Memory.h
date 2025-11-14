#ifndef MEMORY_HPP
#define MEMORY_HPP

struct Node{
    int lchild;
    int rchild;
};

class Memory{
    public:
        // constructor : initializes the free list
        Memory(int size);
        // destructor
        ~Memory();


        // 1. Allocation to the parse tree
        int allocNode();

        // 2. Return to the memory
        void returnOne(int idx); // return one node back to free list
        void returnTree(int root); // return the whole parse tree to free list

        // 3. Getters, Setters
        const Node& getNode(int idx) const {return nodeArr[idx];};
        int getCapacity() const {return capacity;};
        int getFreeRoot() const {return freeRoot;};
        void setLchild(int idx, int elem) {nodeArr[idx].lchild = elem;}
        void setRchild(int idx, int elem) {nodeArr[idx].rchild = elem;}

    private:
        Node* nodeArr; // Node Array
        int capacity; // Capacity of Node Array
        int freeRoot; // root node index of free list(not allocated)
};

#endif