#include "Preprocessor.h"

// concatenate() function: properly insert " " between tokens and concatenate
void concatenate(std::string &s, const std::string &tok)
{
    if(tok.empty()) return;

    // 1. if RPAREN: concat without spacing
    if(tok == ")")
    {
        s += ')';
        return;
    }
    // 2. if LPAREN: add spacing if s is not empty and NOT LPAREN
    else if(tok == "(")
    {
        if(!s.empty() && s.back() != '(')
        {
            s += ' ';
        }
        s += '(';
        return;
    }
    // 3. if normal token: add spacing if s is not empty and NOT LPAREN
    else
    {
        if(!s.empty() && s.back() != '(')
        {
            s += ' ';
        }
        s += tok;
        return;
    }
}

// preprocess() function: modifies the newCommand string which is constructed prior to the function call
void preprocess(Tokenizer &t, std::string& newCommand) 
{
    while(true)
    {
        std::string tok = t.GetNextToken();
        if(tok.empty()) break;

        // 1. token is "define"
        if(tok == "define")
        {
            concatenate(newCommand, "define");
            tok = t.GetNextToken(); // check next token of "define"

            if(tok == "(") // 1) Function ex) (define (square x) (* x x))
            { 
                tok = t.GetNextToken(); // function name
                concatenate(newCommand, tok);
                concatenate(newCommand, "(lambda(");
                preprocess(t, newCommand); // function parameters, body
                concatenate(newCommand, ")");

                tok = t.GetNextToken(); // use the last ')'
            }
            else // 2) NOT a function ex) (define x 3)
            {
                concatenate(newCommand, tok);
            }
        }
        // 2. token is "'""
        else if(tok == "'") // ex) '(1 2 3)
        {
            concatenate(newCommand, "(quote");
            int num_LPAREN = 0;
            do 
            {
                tok = t.GetNextToken();
                concatenate(newCommand, tok);

                if(tok == "(") num_LPAREN++;
                else if(tok == ")") num_LPAREN--;

            } while(num_LPAREN > 0);
            concatenate(newCommand, ")");
        }
        else if(tok.size() > 1 && tok[0] == '\'') // ex) 'error
        {
            concatenate(newCommand, "(quote");
            concatenate(newCommand, tok.substr(1));
            concatenate(newCommand, ")");
        }
        else 
        {
            concatenate(newCommand, tok);
        }
    }
}

