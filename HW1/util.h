#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <netdb.h>
#include <vector>
using namespace std;
string getHost(const char*);
size_t myStrGetTok(const string& str, string& tok, size_t pos = 0,
            const char* del = " ");
void splitString (const string& option, vector<string>& tokens, 
            const char* del = " ");
