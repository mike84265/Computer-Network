#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

class myServer
{
 public:
   myServer();
   myServer(const char* port);
   ~myServer();
   int read(char* buf, size_t len, double timeout=5) const;
   int write(const char* buf, size_t len) const;
   void initSocket();
   void close() { ::close(_listenFd); }
 private:
   // Socket:
   struct sockaddr_in      _serverAddress;
   struct sockaddr_in      _clientAddress;
   char                    _hostname[100];
   unsigned                _port;
   int                     _listenFd;
   mutable fd_set          _rset;
   mutable fd_set          _wset;
};

class myClient
{
 public:
   myClient();
   myClient(const char* hostname, const char* port);
   ~myClient();
   int read(char* buf, size_t len) const;
   int write(const char* buf, size_t len) const;
   void initSocket();
   void close() { ::close(_sockFd); }
 private:
   struct sockaddr_in      _serverAddress;
   struct sockaddr_in      _clientAddress;
   char                    _svrhostname[100];
   int                     _sockFd;
   int                     _port;
   struct hostent*         _hp;
};
