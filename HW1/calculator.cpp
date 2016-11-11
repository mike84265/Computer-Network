#include "calculator.h"
#include <cassert>
#include <cmath>
using namespace std;
double Calculator::operator() (const string& exp)
{
    size_t start=0, len=0;
    ParseState ps = OPERATOR;
    for (size_t i=0;i<exp.size();++i) {
        if (!isValid(exp[i])) {
            string str = "\"";
            str.push_back(exp[i]);
            str += "\" not recognized!";
            throw invalid_argument(str);
            return 0;
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
            if (ps != INTEGER) {
                string str = exp.substr(start,len+1) + ": not a valid float number!";
                throw invalid_argument(str);
            }
            ps = FLOAT;
            ++len;
            break;
          case ' ':
            if (ps == INTEGER || ps == FLOAT) {
                if (len > 0)
                    _numbers.push(stod(exp.substr(start,len)));
                len = 0;
            }
            break;
          case '+': case '*': case '/': case'^':
            if (ps == INTEGER || ps == FLOAT) {
                if (len > 0)
                    _numbers.push(stod(exp.substr(start,len)));
                len = 0;
            } else if (ps != RPAREN) {
                string str = "\"";
                str.push_back(exp[i]);
                str += "\" at position " + to_string(i) + " is invalid!";
                throw invalid_argument(str);
            }
            ps = OPERATOR;
            if (_operators.empty()) {
                _operators.push(exp[i]);
            } else {
                while (!_operators.empty() && compare(exp[i],_operators.top()) <= 0)
                    calTop();
                _operators.push(exp[i]);
            }
            break;
          case '-':
            if (ps == LPAREN || ps==OPERATOR) {
                ps = INTEGER;
                start = i;
                len = 1;
            } else {
                if (ps == INTEGER || ps == FLOAT) {
                    if (len > 0)
                        _numbers.push(stod(exp.substr(start,len)));
                    len = 0;
                }
                ps = OPERATOR;
                // Same as '+'
                if (_operators.empty()) {
                    _operators.push(exp[i]);
                } else {
                    while (!_operators.empty() && compare(exp[i],_operators.top()) <= 0)
                        calTop();
                    _operators.push(exp[i]);
                }
            }
            break;
          case '(': 
            if (ps == INTEGER || ps == FLOAT) {
                if (len > 0)
                    _numbers.push(stod(exp.substr(start,len)));
                len = 0;
            }
            ps = LPAREN;
            _operators.push(exp[i]);
            break;
          case ')':
            if (ps == INTEGER || ps == FLOAT) {
                if (len > 0)
                    _numbers.push(stod(exp.substr(start,len)));
                len = 0;
            }
            while (!_operators.empty() && _operators.top() != '(')
                calTop();
            ps = RPAREN;
            break;
          default:
            cerr << "Warning: Not handled case happens! " << endl;
            break;
        }
    }
    if (ps == INTEGER || ps == FLOAT)
        _numbers.push(stod(exp.substr(start,len)));
    else if (ps != RPAREN) {
        string str = "\"";
        str.push_back(exp.back());
        str += "\" at the end of string is invalid!";
        throw invalid_argument(str);
    }
    while (!_operators.empty()) {
        if (_operators.top() == '(') {
            throw invalid_argument("Parenthesis not paired!");
        }
        calTop();
    }
    return _numbers.top();    
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
            string str = "\"";
            str.push_back(exp[i]);
            str += "\" not recognized!";
            throw invalid_argument(str);
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
            if (!_operators.empty() && compare(exp[i],_operators.top()) <= 0) {
                calTop();
            }
            break;
          case '-':
            if (ps == LPAREN || ps==OPERATOR) {
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
            ps = (exp[i]=='(')? LPAREN : RPAREN;
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

int Calculator::compare(char c1, char c2)
{
    // ()+-*/^
    // () > ^ > */ > +-
    string valOp = "()^*/+-";
    assert(valOp.find_first_of(c1) != string::npos);
    assert(valOp.find_first_of(c1) != string::npos);
    switch(c1) {
      case '^':
        if (c2 == '^')
            return 0;
        else
            return 1;
        break;
      case '*': case '/':
        if (c2 == '^')
            return -1;
        else if (c2 == '*' || c2 == '/')
            return 0;
        else
            return 1;
        break;
      case '+': case '-':
        if (c2 == '^' || c2 == '*' || c2 == '/')
            return -1;
        else if (c2 == '+' || c2 == '-')
            return 0;
        else 
            return 1;
        break;
      case '(': case ')':
        if (c2 == '(' || c2 == ')')
            return 0;
        else
            return -1;
        break;
      default:
        assert(0);
    }
}

double Calculator::calTop()
{
    double n1, n2, ans;
    char op = _operators.top();
    _operators.pop();
    if (op == '(')
        return 0;
    if (!_numbers.empty())
        n2 = _numbers.top();
    else
        throw invalid_argument("Operator error!");
    _numbers.pop();
    if (!_numbers.empty())
        n1 = _numbers.top();
    else
        throw invalid_argument("Parenthesis not paired!");
    _numbers.pop();
    switch(op) {
      case '+':
        ans = n1 + n2;
        #ifdef DEBUG
        printf("%f + %f = %f\n",n1,n2,ans);
        #endif
        break;
      case '-':
        ans = n1 - n2;
        #ifdef DEBUG
        printf("%f - %f = %f\n",n1,n2,ans);
        #endif
        break;
      case '*':
        ans = n1 * n2;
        #ifdef DEBUG
        printf("%f * %f = %f\n",n1,n2,ans);
        #endif
        break;
        return ans;
      case '/':
        ans = n1/n2;
        #ifdef DEBUG
        printf("%f / %f = %f\n",n1,n2,ans);
        #endif
        break;
      case '^':
        ans = pow(n1,n2);
        #ifdef DEBUG
        printf("%f ^ %f = %f\n",n1,n2,ans);
        #endif
        break;
    }
    _numbers.push(ans);
    return ans;
}
