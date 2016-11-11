#include "calculator.h"
#include <cassert>
using namespace std;
double Calculator::operator() (const string& exp)
{
    return 0;    
}

Calculator::~Calculator()
{
    stack<double> temp1;
    _numbers.swap(temp1);
    stack<char> temp2;
    _operators.swap(temp2);
}

void Calculator::parse(const string& exp, vector<double>& ret, vector<char>& op)
{
    size_t start=0, len=0;
    ParseState ps = OPERATOR;
    for (size_t i=0;i<exp.size();++i) {
        if (!isValid(exp[i])) {
            cerr << "Error: \"" << exp[i] << "\" not recognized." << endl;
            return;
        }
        switch(exp[i]) {
          case '0': case '1': case '2': case '3': case '4': 
          case '5': case '6': case '7': case '8': case '9':
            if (ps != INTEGER && ps != FLOAT) {
                ps = INTEGER;
                start = i;
                len = 0;
            }
            ++len;
            break;
          case '.':
            if (ps != INTEGER)
                cerr << "Error : \"" << exp.substr(start,len+1) << "\" is invalid input!" << endl;
            ps = FLOAT;
            ++len;
            break;
          case ' ':
            if (ps == INTEGER || ps == FLOAT) {
                if (len > 0)
                    ret.push_back(stod(exp.substr(start,len)));
                len = 0;
            }
            break;
          case '+': case '*': case '/': case'^':
            if (ps == INTEGER || ps == FLOAT) {
                if (len > 0)
                    ret.push_back(stod(exp.substr(start,len)));
                len = 0;
            }
            ps = OPERATOR;
            op.push_back(exp[i]);
            break;
          case '-':
            if (ps == LPARAN || ps==OPERATOR) {
                ps = INTEGER;
                start = i;
                len = 1;
            } else {
                if (ps == INTEGER || ps == FLOAT) {
                    if (len > 0)
                        ret.push_back(stod(exp.substr(start,len)));
                    len = 0;
                }
                ps = OPERATOR;
                op.push_back(exp[i]);
            }
            break;
          case '(': case ')':
            if (ps == INTEGER || ps == FLOAT) {
                if (len > 0)
                    ret.push_back(stod(exp.substr(start,len)));
                len = 0;
            }
            ps = (exp[i]=='(')? LPARAN : RPARAN;
            op.push_back(exp[i]);
            break;
          default:
            cerr << "Warning: Not handled case happens! " << endl;
            break;
        }
    }
    if (ps == INTEGER || ps == FLOAT)
        ret.push_back(stod(exp.substr(start,len)));
}

bool Calculator::isValid(char c)
{
    for (size_t i=0;i<_validCh.size();++i)
        if (c == _validCh[i])
            return true;
    return false;
}

int compare(char c1, char c2)
{
    // ()+-*/^
    // () > ^ > */ > +-
    string valOp = "()^*/+-";
    assert(valOp.find_first_of(c1) != string::npos);
    assert(valOp.find_first_of(c1) != string::npos);
    switch(c1) {
      case '(': case ')':
        if (c2 == '(' || c2 == ')')
            return 0;
        else
            return 1;
        break;
      case '^':
        if (c2 == '(' || c2 == ')')
            return -1;
        else if (c2 == '^')
            return 0;
        else
            return 1;
        break;
      case '*': case '/':
        if (c2 == '(' || c2 == ')' || c2=='^')
            return -1;
        else if (c2 == '*' || c2 == '/')
            return 0;
        else
            return 1;
        break;
      case '+': case '-':
        if (c2 == '+' || c2 == '-')
            return 0;
        else 
            return -1;
        break;
      default:
        assert(0);
    }
}
