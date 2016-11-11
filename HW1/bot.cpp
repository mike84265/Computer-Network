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
    reply("Hello, I'm" + _userinfo.nick);
    return true;
}

void bot::reply(const string& str) const
{
    string line = "PRIVMSG " + _userinfo.channel + " " + str + "\r\n";
    _socket.write(line.c_str(), line.size());
    cerr << ">>>" << str << endl;
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
        fprintf(stderr,">>> %s\n", _buf);
        // string line(_buf);
        if (isPING()) {
            _buf[1] = 'O';
            _socket.write(_buf,strlen(_buf));
            fprintf(stderr,">>> %s\n",_buf);
        }
        extractMsg();
        if (_line.cmd == "JOIN") {
            string replyMsg = "Hello " + _line.nick + ", welcome to " + _userinfo.channel;
            replyMsg += ", I'm " + _userinfo.nick;
            reply(replyMsg);
        } else if (_line.cmd == "PRIVMSG") {
            size_t n = _line.msg.find_first_of(' ');
            string tok = _line.msg.substr(0,n);
            if (tok == "@repeat") {
                tok = _line.msg.substr(n+1,string::npos);
                reply(tok);
            } else if (tok == "@play") {
                if (_line.msg.substr(n+1,string::npos) == _userinfo.nick)
                _guessNum.init();
                reply("Game start! Guess from 1 to 100 in 5 times");
            } // End for handling @play
            else if (tok == "@guess") {
                if (!_guessNum) {
                    tok = _line.msg.substr(n+1,string::npos);
                    int num;
                    try { num = stoi(tok); }
                    catch (const invalid_argument& ia) {
                        string str = "Invalid input: " + tok;
                        reply(str);
                    }
                    num = _guessNum.compare(num);
                    if (num == 0) {
                        reply("Correct!");
                        _guessNum.clear();
                    } else if (num == GuessNum::OUT_OF_BOUND) {
                        reply("Error! Number out of bound!");
                        ++_guessNum;
                    } else if (num > 0) {
                        string replyMsg = "Less, ";
                        replyMsg += to_string(_guessNum.getRemainNum()) + " times remains.";
                        reply(replyMsg);
                    } else if (num < 0) {
                        string replyMsg = "More, ";
                        replyMsg += to_string(_guessNum.getRemainNum()) + " times remains.";
                        reply(replyMsg);
                    }
                }
            } // End for handling @guess
        }
        bzero(_buf,sizeof(_buf));
    }
    return true;

}

bool bot::isMsg() const
{
    return (strstr(_buf, "PRIVMSG") != NULL);
}

bool bot::isJoin() const
{
    return (strstr(_buf,"JOIN") != NULL && 
      strstr(_buf,"PRIVMSG") == NULL);
}

bool bot::isPING() const
{
    if (strncmp(_buf,"PING",4) == 0)
        return true;
    else
        return false;
}

void bot::extractMsg() 
{
    _line.clear();
    string strbuf(_buf);
    size_t n1,n2;
    n1 = n2 = strbuf.find_first_of('~');
    _line.nick = strbuf.substr(1,n2-2);
    n2 = strbuf.find_first_of(' ');
    ++n1;
    _line.address = strbuf.substr(n1,n2-n1);
    n1 = strbuf.find_first_of(' ',n1);
    n2 = strbuf.find_first_of(' ',++n1);
    _line.cmd = strbuf.substr(n1,n2-n1);
    if (_line.cmd != "PRIVMSG")
        return;
    n1 = strbuf.find_first_of(':',++n1);
    n2 = strbuf.find_first_of("\r\n",n1);
    _line.msg = strbuf.substr(n1+1,n2-n1);
}
