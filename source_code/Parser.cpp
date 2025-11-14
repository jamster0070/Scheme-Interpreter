#include "Parser.h"
#include <iostream>

// constructor
Parser::Parser(Tokenizer& tokenizer, Memory& memory, HashTable& hashtable) 
    : t(tokenizer), m(memory), htab(hashtable) {}

// destructor
Parser::~Parser() {} // no dynamic allocation here

// 1. Read() : read the input and create a parse tree
int Parser::Read()
{
    const int LPAREN = htab.GetHashValue("(");
    const int RPAREN = htab.GetHashValue(")");
    const int END = htab.GetHashValue("");

    int root = 0; // NIL
    int tmp = 0;
    bool isFirst = true;

    std::string tok = t.GetNextToken();
    int tok_hash = htab.GetHashValue(tok);

    // 1. token is LEFT PAREN : scanning start
    if(tok_hash == LPAREN) 
    {
        
        while(true)
        {
            tok = t.GetNextToken(); // get the next token
            tok_hash = htab.GetHashValue(tok); // update hash value to next token

            // if token is RIGHT PAREN, exit the loop
            if(tok_hash == RPAREN)
            {
                break;
            }

            // Wrong Grammar
            if(tok_hash == END)
            {
                throw std::runtime_error("Wrong Grammar, end with ')'");
            }


            if(isFirst)
            {
                tmp = m.allocNode();
                root = tmp;
                isFirst = false;
            }
            else
            {
                int tmpRchild = m.allocNode(); // allocate new node from free list
                m.setRchild(tmp, tmpRchild); // set the rchild of current node as the new node
                tmp = m.getNode(tmp).rchild; // change tmp to new node(current)
            }
            
            if(tok_hash == LPAREN) // nested list 
            {
                t.PushBack(); // roll back to before the new LEFT PAREN
                int tmpLchild = Read(); // recursively call Read(), tmpLchild is the root of the subtree(nested list)
                m.setLchild(tmp, tmpLchild); // set the lchild of current node as the root of its subtree
            }
            else // SYMBOL
            {
                htab.HashInsert(tok);
                int hashValue = htab.GetHashValue(tok);
                m.setLchild(tmp, hashValue);
            }

            // as soon as allocated, the new node's rchild is set to 0(NIL) : already implemented in Memory::allocNode()
        }

        return root;
    }
    // 2. token is RIGHT PAREN
    else if(tok_hash == RPAREN)
    {
        throw std::runtime_error("Wrong Grammar, start with '('");
    }
    // 3. token is NULL STRING : GetHashValue() returns 0
    else if(tok_hash == END) 
    {
        return END; // end
    }
    // 4. token is a SYMBOL that is NOT LEFT PAREN or RIGHT PAREN 
    // GetHashValue() returns -1 ~ -(capacity-1) : SYMBOL is found 
    // GetHashValue() returns -capacity : SYMBOL is NOT found
    else 
    {
        htab.HashInsert(tok); // HashInsert() already deals with the case when SYMBOL is found : don't insert
        int hashValue = htab.GetHashValue(tok);

        return hashValue;
    }
}

// 2. Print() : read the parse tree and print the original input
void Parser::Print(int root, bool startList)
{
    // End Condition 1. root is NIL
    if(root == 0) 
    {
        std::cout << "()";
    }
    // End Condition 2. root is symbol
    else if(root < 0) 
    {
        std::cout << htab.FindSymbol(root);
    }
    // Recursive : root is node index
    else // 
    {
        // if new list : print "("" first
        if(startList) {std::cout << "(";}

        // print current node's left child, flag is true
        Print(m.getNode(root).lchild, true);

        // if next node exists : print " ", then print right child, flag is false
        if(m.getNode(root).rchild != 0)
        {
            std::cout << " ";
            Print(m.getNode(root).rchild, false);
        }
        // if next node doesn't exist  : print ")" then stop (end of the list)
        else
        {
            std::cout << ")";
        }
    }
}

