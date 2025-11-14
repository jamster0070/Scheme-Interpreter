#include "Tokenizer.h"

Tokenizer::Tokenizer(const std::string& input) : cmd(input), size(input.length()), curr(0), prev(0)
{}

void Tokenizer::skipSpace()
{
    while(curr < size && cmd[curr] == ' ')
    {
        curr++;
    }
}

// 1. GetNextToken() : move curr to the end of the next token and return the next token
std::string Tokenizer::GetNextToken()
{
    skipSpace();

    if(curr >= size)
    {
        return std::string("");
    }

    prev = curr; // for PushBack()

    if(cmd[curr] == '(') // Left Parenthesis -> used later for parse tree
    {
        curr++;
        return std::string("("); 
    }
    else if(cmd[curr] == ')') // Right Parenthesis -> used later for parse tree
    {
        curr++;
        return std::string(")"); 
    }
    else // Symbols to be stored in the hash table
    {
        int start = curr;
        int count = 0;
        while(curr < size && cmd[curr] != ' ' && cmd[curr] != '(' && cmd[curr] != ')')
        {
            curr++;
            count++;
        }
        std::string tmp = cmd.substr(start, count);

        int length = tmp.length();
        // lower case the symbol
        for(int i = 0; i < length; i++)
        {
            tmp[i] = tolower(tmp[i]);
        }

        // if length exceeds 10, only take the first 10 chars
        if(length > 10) 
        {
            return tmp.substr(0, 10);
        }
        else 
        {
            return tmp;
        }
    }
}

// 2. PushBack() : GetNextToken() -> PushBack(), roll back to the position before GetNextToken()
void Tokenizer::PushBack()
{
    curr = prev;
}