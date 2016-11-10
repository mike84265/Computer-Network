#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include "util.h"
#include "mySocket.h"
using namespace std;
int main()
{
    mySocket skt("irc.rizon.net","6667");
    char buf[512];
    fprintf(stderr,"Connecting ...\n");
    if (skt.connect() < 0) {
        fprintf(stderr, "Connection error\n");
        exit(1);
    }
    fprintf(stderr,"testpoint\n");
    while (skt.read(buf,sizeof(buf)) > 0) {
        fprintf(stderr, "Read --- %s ---\n",buf);
        bzero(buf,sizeof(buf));
    }
    fprintf(stderr,"Join channel\n");
    ifstream config("config",ios::in);
    skt.setChannel(config);
    while (skt.read(buf,sizeof(buf),-1) > 0) {
        fprintf(stderr,"buf = %s\n", buf);
        string line(buf);
        vector<string> tok;
        splitString(line,tok," ");
        for (size_t i=0;i<tok.size();++i)
            cout << "tok[" << i << "] = " << tok[i] << endl;
        if (tok[0] == "PING"){
            skt.write("PONG ", 4);
            skt.write(tok[1].c_str(), tok[1].size());
        } else {
            skt.reply(tok[3]);
        }
        bzero(buf,sizeof(buf));
    }
    // :b03901078!~b03901078@Rizon-C14D6386.csie.ntu.edu.tw PRIVMSG #CN1103 :QAQ
    // :(NICK)!~(Host) PRIVMSG (CHAN) :(MSG)
}
