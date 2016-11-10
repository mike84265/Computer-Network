#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <util.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
using namespace std;
class mySocket
{
  public:
    mySocket();
    mySocket(const char*, const char*);
    ~mySocket();
    int connect();
    int read(char*,size_t, double timeout=2.5) const;
    int write(const char*, size_t) const;
    void close() { ::close(_sockFd); }
    void setChannel(ifstream&);
    void setNick(const string&);
    void reply(const string&) const;
  private:
    void initSocket();
    // Socket:
    struct sockaddr_in      _serverAddress;
    string                  _hostname;
    unsigned                _port;
    int                     _sockFd;
    mutable fd_set          _rset;
    mutable fd_set          _wset;
    // IRC:
    string                  _nick;
    string                  _user;
    string                  _bla;
    string                  _channel;
    string                  _password;
};
