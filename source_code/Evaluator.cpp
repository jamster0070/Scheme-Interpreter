#include "Evaluator.h"
#include "ElemStack.h"

#include <iostream>
#include <cctype> // for isdigit

// constructor
Evaluator::Evaluator(Memory& memory, HashTable& hashtable):m(memory), htab(hashtable)
{
    // if the symbols are not inserted yet, all the values will be 0
    // since Parser::read() is called before Evaluator::eval(), the symbols of the command will be inserted into the hash table 
    PLUS = htab.GetHashValue("+");
    MINUS = htab.GetHashValue("-");
    MULT = htab.GetHashValue("*");
    DIV = htab.GetHashValue("/");

    ISNUMBER = htab.GetHashValue("number?");
    ISNULL = htab.GetHashValue("null?");

    CONS = htab.GetHashValue("cons");
    CAR = htab.GetHashValue("car");
    CDR = htab.GetHashValue("cdr");

    COND = htab.GetHashValue("cond");
    QUOTE = htab.GetHashValue("quote");
    DEFINE = htab.GetHashValue("define");
    LAMBDA = htab.GetHashValue("lambda");
    ELSE = htab.GetHashValue("else");

    // #t and #f doesn't usually appear in the command
    // HashTable::BuiltIn() calls HashInsert("#t") and HashInsert("#f");
    TRUE_SYM = htab.GetHashValue("#t");
    FALSE_SYM = htab.GetHashValue("#f");
}

// 1. Eval() : reads the root of a parse tree and returns the result of evaluation
// 1) return negative int or 0 : hash value(result is a symbol, number, or #t, #f)
// 2) return positive int : node index(result is a list)
int Evaluator::Eval(int root)
{
    // root is 0 : NIL
    if(root == 0) return 0;

    // root < 0 : hash value (symbol/number/#t/#f)
    if(root < 0)
    {
        Element& elem = htab.getElem(-root);
        int link = elem.linkOfValue;
        int markNIL = -htab.getCapacity();

        if(link == markNIL) // ex) (define y '())
        {
            return 0; // (null? y) -> true
        }
        else if(link != 0) // there is a link to this value ex) (define x 3)
        {
            return link;
        }
        else // link == 0: can be undefined variable or number/#t/#f
        {
            if(IsNumber(root)) {return root;} // number
            else if(root == TRUE_SYM || root == FALSE_SYM) {return root;} // #t or #f
            else {throw std::runtime_error("Unbound variable: " + elem.symbol);} // undefined variable
        }
    }

    // root > 0
    // 1. get the operator token in the first cell
    int tokenIndex = m.getNode(root).lchild;

    // 2. (+ a b)
    if(tokenIndex == PLUS)
    {
        int first = m.getNode(root).rchild;
        int second = m.getNode(first).rchild;

        int firstRoot = m.getNode(first).lchild;
        int secondRoot = m.getNode(second).lchild;

        double v1 = GetVal(Eval(firstRoot));
        double v2 = GetVal(Eval(secondRoot));

        return MakeNumber(v1 + v2);
    }
    // 3. (- a b)
    else if(tokenIndex == MINUS)
    {
        int first = m.getNode(root).rchild;
        int second = m.getNode(first).rchild;

        int firstRoot = m.getNode(first).lchild;
        int secondRoot = m.getNode(second).lchild;

        double v1 = GetVal(Eval(firstRoot));
        double v2 = GetVal(Eval(secondRoot));

        return MakeNumber(v1 - v2);
    }
    // 4. (* a b)
    else if(tokenIndex == MULT)
    {
        int first = m.getNode(root).rchild;
        int second = m.getNode(first).rchild;

        int firstRoot = m.getNode(first).lchild;
        int secondRoot = m.getNode(second).lchild;

        double v1 = GetVal(Eval(firstRoot));
        double v2 = GetVal(Eval(secondRoot));

        return MakeNumber(v1 * v2);
    }
    // 5. (/ a b)
    else if(tokenIndex == DIV)
    {
        int first = m.getNode(root).rchild;
        int second = m.getNode(first).rchild;

        int firstRoot = m.getNode(first).lchild;
        int secondRoot = m.getNode(second).lchild;

        double v1 = GetVal(Eval(firstRoot));
        double v2 = GetVal(Eval(secondRoot));

        // assume that b is not zero in every command
        return MakeNumber(v1 / v2);
    }
    // 6. (number? x)
    else if(tokenIndex == ISNUMBER)
    {
        int argCons = m.getNode(root).rchild;
        int argRoot = m.getNode(argCons).lchild;
        
        int val = Eval(argRoot);

        if(IsNumber(val)) return TRUE_SYM;
        else return FALSE_SYM;
    }
    // symbol? 은 구현에서 제외
    // 7. (null? x)
    else if(tokenIndex == ISNULL)
    {
        int argCons = m.getNode(root).rchild;

        if(argCons == 0) return TRUE_SYM;

        int argRoot = m.getNode(argCons).lchild; // where x is
        int val = Eval(argRoot);

        if(val == 0) return TRUE_SYM;
        else return FALSE_SYM;
    }
    // 8. (cons a b)
    else if(tokenIndex == CONS)
    {
        int first = m.getNode(root).rchild;
        int second = m.getNode(first).rchild;

        int firstRoot = m.getNode(first).lchild;
        int secondRoot = m.getNode(second).lchild;

        int firstVal = Eval(firstRoot);
        int secondVal = Eval(secondRoot);

        // allocate new cons cell
        int newMemory = m.allocNode();
        m.setLchild(newMemory, firstVal);
        m.setRchild(newMemory, secondVal);

        return newMemory;
    }
    // 9. (cond ((C1) (E1)) ... ((Cn) (En)) (else (Eelse)))
    else if(tokenIndex == COND)
    {
        int curr = root; // points to the node whose rchild chain tracking

        // evaluate except last one
        while(m.getNode(m.getNode(curr).rchild).rchild != 0)
        {
            curr = m.getNode(curr).rchild;
            int condCons = m.getNode(curr).lchild;
            int condRoot = m.getNode(condCons).lchild;

            int exprCons = m.getNode(condCons).rchild;
            int exprRoot = m.getNode(exprCons).lchild;

            if(Eval(condRoot) == TRUE_SYM)
            {
                return Eval(exprRoot);
            }
        }

        // evaluate the last one (else)
        int lastCons = m.getNode(curr).rchild;

        int elseCons = m.getNode(lastCons).lchild;
        int elseRoot = m.getNode(elseCons).lchild; // should be the hash value of 'else'
        if(elseRoot != ELSE) throw std::runtime_error("[cond] last clause must be 'else'");

        int elseExpCons = m.getNode(elseCons).rchild; // (Eelse)
        int elseExprRoot = m.getNode(elseExpCons).lchild;

        return Eval(elseExprRoot);
    }
    // 10. (car x)
    else if(tokenIndex == CAR)
    {
        int argCons = m.getNode(root).rchild;
        int argRoot = m.getNode(argCons).lchild;

        int listVal = Eval(argRoot);
        return m.getNode(listVal).lchild; // first element
    }
    // 11. (cdr x)
    else if(tokenIndex == CDR)
    {
        int argCons = m.getNode(root).rchild;
        int argRoot = m.getNode(argCons).lchild;

        int listVal = Eval(argRoot);
        return m.getNode(listVal).rchild; // except the first element
    }
    // 12. (define ...)
    else if(tokenIndex == DEFINE)
    {
        // i) user defined function (define square (lambda(x) (* x x)))
        // ii) variable declaration (define x 3)
        int nameCons = m.getNode(root).rchild;
        int nameHash = m.getNode(nameCons).lchild; // i) function name or ii)variable name
        int valueCons = m.getNode(nameCons).rchild;
        int valueRoot = m.getNode(valueCons).lchild; // i) lambda or ii) expression

        int storedVal;

        if(valueRoot > 0 && m.getNode(valueRoot).lchild == LAMBDA) 
        {
            storedVal = valueRoot; // case i)
        }
        else 
        {
            storedVal = Eval(valueRoot); // case ii)
        }

        Element& nameElem = htab.getElem(-nameHash);

        // #### special marker for NIL ex) (define y '()) -> should have a link that is NOT 0
        int markNIL = -htab.getCapacity();

        if(storedVal == 0) nameElem.linkOfValue = markNIL;
        else nameElem.linkOfValue = storedVal; 

        return nameHash;
    }
    // 13. (quote x)
    else if(tokenIndex == QUOTE)
    {
        int argCons = m.getNode(root).rchild;
        int quoteRoot = m.getNode(argCons).lchild;

        return quoteRoot;
    }
    // 14. otherwise: user defined function call ex) (square 6) 
    else
    {
        int argRoot = m.getNode(root).rchild;
        return UserFunc(tokenIndex, argRoot); // tokenindex is function name
    }
}

// 2. PrintResult() : prints the result of evaluation
void Evaluator::PrintResult(int result, bool startlist)
{
    // 1. NIL (empty list)
    if(result == 0)
    {
        std::cout << "()";
        return;
    }
    // 2. Atom (symbol / number / #t / #f)
    else if(result < 0) 
    {
        std::cout << htab.FindSymbol(result);
        return;
    }
    // 3. List: cons cell
    else // result > 0
    {
        if(startlist) std::cout << "(";

        // print first element
        int car = m.getNode(result).lchild;
        PrintResult(car, true);

        // print leftover
        int cdr = m.getNode(result).rchild;
        if(cdr != 0)
        {
            std::cout << " ";
            PrintResult(cdr, false);
        }
        else // end of list
        {
            std::cout << ")";
        }
    }
}

// 3. UserFunc() : helper function for calling user defined functions 
// restoring the original value(use ElemStack)
// funcExp : hash value of name(symbol) of the function
// argRoot : node index that points to the function argument
int Evaluator::UserFunc(int funcExp, int argRoot)
{
    Element& funcElem = htab.getElem(-funcExp);
    int lambdaRoot = funcElem.linkOfValue; 

    // [lambdaRoot].lchild = 'lambda'
    // [lambdaRoot].rchild = cons(paramList funcBody)
    int cons = m.getNode(lambdaRoot).rchild;
    int paramRoot = m.getNode(cons).lchild;
    int bodyCons = m.getNode(cons).rchild;
    int bodyRoot = m.getNode(bodyCons).lchild;

    ElemStack backupStack(htab.getCapacity());

    while(paramRoot !=0 && argRoot != 0)
    {
        // current paramenter symbol hash
        int paramHash = m.getNode(paramRoot).lchild;

        // current argument expression root
        int argExprRoot = m.getNode(argRoot).lchild;
        int argVal = Eval(argExprRoot); // can be hash or node index

        Element& paramElem = htab.getElem(-paramHash);

        // back up the copy of the current Element into the stack
        Element backup = paramElem;
        backupStack.Push(backup);

        paramElem.linkOfValue = argVal; // temporary update of link

        // move to next parameter, argument
        paramRoot = m.getNode(paramRoot).rchild;
        argRoot = m.getNode(argRoot).rchild;
    }

    int result = Eval(bodyRoot);

    // Restore original linkOfValue for all parameters(Pop until backupStack is empty)
    {
        Element backup = backupStack.Top();
        backupStack.Pop();

        Element& e = htab.getElem(-backup.hashValue);
        e.linkOfValue = backup.linkOfValue;
    }
    return result;
}

 // 4. IsNumber() : check if the result(hash value) is a number 
bool Evaluator::IsNumber(int value) const
{
    if(value >= 0) return false; // node index or NIL : cannot be a number

    std::string s = htab.FindSymbol(value);

    if(s.empty()) return false;

    int i = 0;
    bool hasDigit = false;
    bool hasDot = false;
    
    int l = s.size();

    // check unary operators(sign)
    if(s[0] == '+' || s[0] == '-')
    {
        if(l == 1) return false; // only "+" or "-" is not a number
        i = 1;
    }

    // check the value
    for(; i < l; i++)
    {
        if(std::isdigit(s[i]))
        {
            hasDigit = true;
        }
        else if (s[i] == '.' && !hasDot)
        {
            // allow one decimal point(only one!)
            hasDot = true;
        }
        else {return false;} // no numbers
    }

    // at least one digit is required
    return hasDigit;
}

// 5. GetVal() : get the result(hash value) and return the symbol(number) in double
double Evaluator::GetVal(int value) const
{
    std::string s = htab.FindSymbol(value);

    int i = 0;
    bool negative = false;

    int l = s.size();

    if(!s.empty() && (s[0] == '+' || s[0] == '-'))
    {
        negative = (s[0] == '-');
        i = 1;
    }

    double result = 0.0;

    // integer part
    while(i < l && std::isdigit(s[i]))
    {
        result = result * 10.0 + (s[i] - '0');
        i++;
    }

    // decimal part
    if(i < l && s[i] == '.')
    {
        i++;
        double decimal = 0.1;

        while(i < l && std::isdigit(s[i]))
        {
            result = result + (s[i] - '0') * decimal;
            decimal *= 0.1;
            i++;
        }
    }

    if(negative) result = -result;
    return result;
}

// 6. MakeNumber() : change the calculation result(double) into a symbol and insert, then return its hash value
int Evaluator::MakeNumber(double value)
{
    // convert double to string ex) "3.50000"
    std::string s = std::to_string(value);

    // remove excess zeros after decimal
    int l = s.size();
    int pos = -1; // position of the dot

    for(int i = 0; i < l; i++)
    {
        if(s[i] == '.')
        {
            pos = i;
            break;
        }
    }

    if(pos != -1) // dot exists!
    {
        int last = l - 1;

        // remove trailing '0'
        while(last > pos && s[last] == '0'){last--;}

        if(last == pos){last--;}

        s = s.substr(0, last + 1);
    }

    // insert this string into the hash table and return the hash value
    htab.HashInsert(s);
    int h = htab.GetHashValue(s);

    return h;
}

