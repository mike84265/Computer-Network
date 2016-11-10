#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include "mySocket.h"
#include "util.h"
#ifndef BOT_H
#define BOT_H
using namespace std;
class bot
{
 public:
    bot();
    bot(const char*, const char*);
    void setInfo(const string& nick, const string& user);
    bool getConfig(ifstream&);
    void reply(const string&) const;
    bool handleMsg();
    void display() const;
 private:
    mySocket                _socket;
    UserInfo                _userinfo;
    mutable char            _buf[1024];
};
#endif
