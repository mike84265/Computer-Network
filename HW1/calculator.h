#include <iostream>
#include <cctype>
#include <string>
#include <stack>
#include <vector>
#ifndef CALCULATOR_H
#define CALCULATOR_H
using namespace std;
enum ParseState {
    OPERATOR,
    INTEGER,
    FLOAT,
    LPAREN,
    RPAREN
};
class Calculator
{
 public:
    Calculator() : _validCh("0123456789.+-*/^() ") {}
    ~Calculator();
    double operator() (const string& exp);
    void parse(const string& exp, vector<double>& ret, vector<char>& op);
 private:
    bool isValid(char c);
    int compare(char c1, char c2);
    double calTop();

    // Member variables:
    const string                _validCh;
    string                      _exp;
    stack<double>               _numbers;
    stack<char>                 _operators;
    stack<char>                 _parenthesis;
};
#endif
