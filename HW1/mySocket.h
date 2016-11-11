#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include "util.h"
#ifndef MYSOCKET_H
#define MYSOCKET_H
using namespace std;
class mySocket
{
  public:
    mySocket();
    mySocket(const char*, const char*);
    ~mySocket();
    int connect(UserInfo&);
    int read(char*,size_t, double timeout=1.2) const;
    int write(const char*, size_t) const;
    void close() { ::close(_sockFd); }
    void joinChannel(UserInfo&);
  private:
    void initSocket();
    // Socket:
    struct sockaddr_in      _serverAddress;
    string                  _hostname;
    unsigned                _port;
    int                     _sockFd;
    mutable fd_set          _rset;
    mutable fd_set          _wset;
};
#endif
