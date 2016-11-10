#include "bot.h"
bot::bot() 
    :_userinfo({"MikeTsai","Mike","",""})
{
    _socket.connect(_userinfo);
    display();
}

bot::bot(const char* hostname, const char* port)
    :_socket(hostname,port),
    _userinfo({"MikeTsai","Mike","",""})
{
    _socket.connect(_userinfo);
    display();
}

bool bot::getConfig(ifstream& fin) {
    string buf;
    getline(fin,buf);
    size_t n1 = buf.find_first_of('\'');
    size_t n2 = buf.find_first_of('\'',n1+1);
    _userinfo.channel = buf.substr(n1+1,n2-n1-1);
    if (_userinfo.channel.size() == 0)
        return false;
    getline(fin,buf);
    if (buf.size() != 0) {
        n1 = buf.find_first_of('\'');
        n2 = buf.find_first_of('\'',n1+1);
        _userinfo.password = buf.substr(n1+1,n2-n1-1);
    }
    _socket.joinChannel(_userinfo);
    return true;
}

void bot::reply(const string& str) const
{
    string line = "PRIVMSG " + _userinfo.channel + " " + str + "\r\n";
    _socket.write(line.c_str(), line.size());
}

void bot::display() const
{
    while(_socket.read(_buf,sizeof(_buf)) > 0) {
        fprintf(stderr,">>>%s\n",_buf);
        bzero(_buf,sizeof(_buf));
    }
}
bool bot::handleMsg()
{
    while (_socket.read(_buf,sizeof(_buf),-1)) {
        fprintf(stderr,"buf = %s\n", _buf);
        string line(_buf);
        bzero(_buf,sizeof(_buf));
    }
    return true;

}
