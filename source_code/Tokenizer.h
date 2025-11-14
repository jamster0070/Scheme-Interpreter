#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>

class Tokenizer {
    public:
        std::string cmd;
        int size;
        Tokenizer(const std::string& input);
        void skipSpace();

        // 1. GetNextToken() : move curr to the end of the next token and return the next token
        std::string GetNextToken();
        // 2. PushBack() : GetNextToken() -> PushBack(), roll back to the position before GetNextToken()
        void PushBack();

    private:
        int curr;
        int prev;
};

#endif