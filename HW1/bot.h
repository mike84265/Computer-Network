#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include "mySocket.h"
#include "util.h"
#include "GuessNum.h"
#ifndef BOT_H
#define BOT_H
using namespace std;
class bot
{
 public:
    bot();
    bot(const char* hostname, const char* port);
    void setInfo(const string& nick, const string& user);
    bool getConfig(ifstream& fin);
    void reply(const string& str) const;
    bool handleMsg();
    void display() const;
 private:
    // Helper functions for handling messages:
    // Reading _buf to make decisions.
    bool isMsg() const;
    bool isJoin() const;
    bool isPING() const;
    string extractMsg() const;
    // Member variables:
    mySocket                _socket;
    UserInfo                _userinfo;
    mutable char            _buf[1024];
    GuessNum                _guessNum; 
};
#endif
