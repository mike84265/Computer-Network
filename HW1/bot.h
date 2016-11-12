#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <string.h>
#include "mySocket.h"
#include "util.h"
#include "GuessNum.h"
#include "calculator.h"
#ifndef BOT_H
#define BOT_H
using namespace std;
class bot
{
 public:
    bot(const char* hostname="irc.freenode.net", const char* port="6667",
        const char* nick="MikeTsai", const char* user="Mike");
    void setInfo(const string& nick, const string& user);
    bool getConfig(ifstream& fin);
    void reply(const string& str) const;
    bool handleMsg();
    void display() const;
 private:
    // Helper functions for handling messages:
    // Reading _buf to make decisions.
    bool isPING() const;
    void extractMsg();
    void initHelpMsg();
    // Member variables:
    mySocket                _socket;
    UserInfo                _userinfo;
    Line                    _line;
    mutable char            _buf[1024];
    GuessNum                _guessNum; 
    Calculator              _calculator;
    map<string,string>      _help;
};
#endif
