#include "mySocket.h"
#include "util.h"
#include <sys/socket.h>
#include <iostream>
#define ERR_EXIT(x) {   \
    fprintf(stderr,x);  \
    exit(1); }
using namespace std;

mySocket::mySocket() : _hostname("irc.freenode.net"), _port(6667)
{
    initSocket();
}

mySocket::mySocket(const char* hostname, const char* port) :
    _hostname(hostname), _port(atoi(port)) 
{
    initSocket();
}

void mySocket::initSocket()
{
    _sockFd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&_serverAddress, sizeof(_serverAddress));
    _serverAddress.sin_family = AF_INET;
    _serverAddress.sin_port = htons(_port);     
    inet_pton(AF_INET, getHost(_hostname.c_str()).c_str(),&_serverAddress.sin_addr);
    FD_ZERO(&_rset);
    FD_ZERO(&_wset);
    FD_SET(_sockFd,&_rset);
    FD_SET(_sockFd,&_wset);
}

mySocket::~mySocket()
{
    this->close();
}
int mySocket::connect(UserInfo& info)
{
    cerr << "Start to connect..." << endl;
    if (::connect(_sockFd, (struct sockaddr*)&_serverAddress, 
       sizeof(_serverAddress)) < 0)
        ERR_EXIT("Connection Error!\n");
    char buf[512];
    if (this->read(buf,512) > 0) {
        vector<string> lines;
        splitString(string(buf),lines,"\r\n");
        for (size_t i=0;i<lines.size();++i){
            cerr << "--Read " << i << ": "; 
            cerr << lines[i] << endl;
            #ifdef DEBUG
            for (size_t j=0;j<lines[i].size();++j)
                cerr << int(lines[i][j]) << " ";
            cerr << "--\n";
            #endif
        }
    }
    string nick = "NICK " + info.nick + "\r\n";
    string user = "USER " + info.user + " " + _hostname + " " + info.nick + " :" + info.user + "\r\n";
    cerr << "write nick..." << nick << endl;
    if (this->write(nick.c_str(), nick.size()) < 0)
        ERR_EXIT("Sending Nick Error\n");
    bzero(buf,512);
    cerr << nick.c_str() << endl << buf << endl << endl;
    this->read(buf,512);
    if (strncmp(buf,"PING",4) == 0) {
        cerr << "PING..." << endl;
        cerr << buf << endl;
        buf[1] = 'O';
        write(buf,strlen(buf));
        cerr << buf << endl;
        write(nick.c_str(), nick.size());
        cerr << nick << endl;
    }
    cerr << "write user..." << user << endl;
    if (this->write(user.c_str(), user.size()) < 0)
        ERR_EXIT("Sending User Error\n");
    cerr << user.c_str() << endl << buf << endl << endl;
    bzero(buf,512);
    this->read(buf,512);
    if (strncmp(buf,"PING",4) == 0) {
        cerr << "PING..." << endl;
        cerr << buf << endl;
        buf[1] = 'O';
        write(buf,strlen(buf));
        cerr << buf << endl;
        write(user.c_str(), user.size());
        cerr << user << endl;
    }
    return 0;
}
int mySocket::read(char* buf, size_t len, double timeout) const
{
    if (timeout < 0)
    return ::recv(_sockFd, buf, len,0);
    int n;
    struct timeval tv;
    tv.tv_sec = int(timeout);
    tv.tv_usec = int(timeout * 1000000) % 1000000;
    n = select(4,&_rset,NULL,NULL,&tv);
        
    #ifdef DEBUG
    fprintf(stderr,"select(r) = %d\n",n);
    #endif
    if (n == 0)
        return 0;
    return ::recv(_sockFd, buf, len,0);
}
int mySocket::write(const char* buf, size_t len) const
{
    #ifdef DEBUG
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 500000;
    int n = select(4,NULL,&_wset,NULL,&tv);
    fprintf(stderr,"select(w) = %d\n",n);
    #endif
    return ::send(_sockFd, buf, len,0);    
}

void mySocket::joinChannel(UserInfo& userinfo)
{
    string msg = "JOIN " + userinfo.channel;
    if (userinfo.password.size() != 0) {
        msg += " ";
        msg += userinfo.password;
    }
    msg += "\r\n";
    if (this->write(msg.c_str(), msg.size()) <= 0)
        ERR_EXIT("JOIN Error\n");
    cerr << "Joining to channel..." << endl;
    cerr << msg << endl;
    // Error handling not considered yet.
}

