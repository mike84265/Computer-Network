#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>

#define PACKET_SIZE 128
#define BUFFER_SIZE 32

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
   int filedes() const { return _listenFd; }
 private:
   // Socket:
   struct sockaddr_in      _serverAddress;
   struct sockaddr_in      _clientAddress;
   char                    _hostname[100];
   unsigned                _port;
   int                     _listenFd;
   mutable fd_set          _rset;
};

class myClient
{
 public:
   myClient();
   myClient(const char* hostname, const char* port);
   ~myClient();
   int read(char* buf, size_t len, double timeout=3) const;
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
   mutable fd_set          _rset;
};

typedef struct
{
   short num;
   short nbytes;
   char buf[PACKET_SIZE];
} Data;


class Buffer
{
 public:
   Buffer();
   ~Buffer() {}
   Data& operator[] (int i) { return _data[i]; }
   const Data& operator[] (int i) const { return _data[i]; }
   size_t size() const { return _size; }
   bool full() const { return (_size >= BUFFER_SIZE); }
   bool empty() const { return (_size == 0); }
   void clear();
   int push(const Data& data);

 private:
   Data                    _data[BUFFER_SIZE];
   size_t                  _size;
};
