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
#include "bot.h"
using namespace std;
int main()
{
    bot bot("irc.rizon.net","6667");
    bot.display();
    fprintf(stderr,"Join channel\n");
    ifstream config("config",ios::in);
    bot.getConfig(config);
    bot.handleMsg();
    // :b03901078!~b03901078@Rizon-C14D6386.csie.ntu.edu.tw PRIVMSG #CN1103 :QAQ
    // :(NICK)!~(Host) PRIVMSG (CHAN) :(MSG)
}
