#include "mySocket.h"
#include <string.h>
#define ERR_EXIT(str)         \
   { fprintf(stderr,"%s",(str)); exit(1); }

myServer::myServer() : _port(9990)
{
   gethostname(_hostname, sizeof(_hostname));
   initSocket();
}

myServer::myServer(const char* port) : _port(atoi(port))
{
   gethostname(_hostname, sizeof(_hostname));
   initSocket();
}

myServer::~myServer()
{
   close();
}

void myServer::initSocket()
{
   if ( (_listenFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
      ERR_EXIT("socket in server failed.\n");
   bzero(&_serverAddress, sizeof(_serverAddress));
   _serverAddress.sin_family = AF_INET;
   _serverAddress.sin_port = htons(_port);
   _serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
   if ( bind(_listenFd, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress)) < 0)
      ERR_EXIT("bind in server failed.\n");
}

int myServer::read(char* buf, size_t len, double timeout) const
{
   socklen_t length = sizeof(_clientAddress);
   int n, nbytes;
   struct timeval tv;

   FD_ZERO(&_rset);
   FD_SET(_listenFd, &_rset);

   if (timeout >= 0) {
      tv.tv_sec = int(timeout);
      tv.tv_usec = int(timeout * 1000000) % 1000000;
      n = select(_listenFd+1,&_rset,NULL,NULL,&tv);
   } else
      n = select(_listenFd+1,&_rset,NULL,NULL,NULL);
   if (n == 0)
       return -1;
   nbytes = ::recvfrom(_listenFd, buf, len, 0, (struct sockaddr*)&_clientAddress, &length); 
   return nbytes;
}

int myServer::write(const char* buf, size_t len) const
{
   int nbytes;
   socklen_t length = sizeof(_clientAddress);
   nbytes = sendto(_listenFd, buf, len, 0, (struct sockaddr*)&_clientAddress, length);
   return nbytes;
}

myClient::myClient() : _port(9990)
{
   initSocket();
}

myClient::myClient(const char* hostname, const char* port) : _port(atoi(port))
{
   strcpy(_svrhostname, hostname);
   initSocket();
}

myClient::~myClient()
{
   close();
}
void myClient::initSocket()
{
   if ( (_sockFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
      ERR_EXIT("socket in client failed\n");
   bzero(&_clientAddress, sizeof(_clientAddress));
   _clientAddress.sin_family = AF_INET;
   _clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
   _clientAddress.sin_port = htons(0);
   if (bind(_sockFd, (struct sockaddr*)&_clientAddress, sizeof(_clientAddress)) < 0)
      ERR_EXIT("bind in client failed\n");
   bzero(&_serverAddress, sizeof(_serverAddress));
   _serverAddress.sin_family = AF_INET;
   _serverAddress.sin_port = htons(_port);
   _hp = gethostbyname(_svrhostname);
   if (_hp == 0)
      ERR_EXIT("gethost failed\n");
   bcopy(_hp->h_addr_list[0], (caddr_t)&_serverAddress.sin_addr, _hp->h_length);
}

int myClient::read(char* buf, size_t len, double timeout) const
{
   socklen_t size = sizeof(_serverAddress);
   int n;
   struct timeval tv;

   FD_ZERO(&_rset);
   FD_SET(_sockFd, &_rset);

   if (timeout >= 0) {
      tv.tv_sec = int(timeout);
      tv.tv_usec = int(timeout * 1000000) % 1000000;
      n = select(_sockFd+1,&_rset,NULL,NULL,&tv);
   } else
      n = select(_sockFd+1,&_rset,NULL,NULL,NULL);
   if (n == 0)
       return -1;

   int nbytes = recvfrom(_sockFd, buf, len, 0, (struct sockaddr*)&_serverAddress, &size);
   return nbytes;
}

int myClient::write(const char* buf, size_t len) const
{
   return ::sendto(_sockFd, buf, len, 0, (struct sockaddr*)&_serverAddress, sizeof(_serverAddress));
}

void Buffer::clear()
{
   _data[0].num = _data[_size-1].num + 1;
   _size = 0;
   fprintf(stderr,"buffer is cleared. _data[0].num is set to %d\n",_data[0].num);
}

int Buffer::push(const Data& data)
{
   int n;
   if ( ( n = data.num - _data[0].num) >= BUFFER_SIZE)
      return -2;
   else {
      if (_data[n].num == data.num && data.num != 0)
         return -1;
      else 
         _data[n] = data;
   }
   ++_size;
   return 0;
}
