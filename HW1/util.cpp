#include "util.h"
using namespace std;
string getHost(const char* hostname)
{
    hostent* phe = gethostbyname(hostname);
    if (phe == NULL) {
        fprintf(stderr, "%s is unavailable\n", hostname);
        exit(1);
    }
    in_addr* address = (in_addr*)phe->h_addr;
    return inet_ntoa(*address);
}

size_t myStrGetTok(const string& str, string& tok, size_t pos,
            const char* del)
{
   size_t begin = str.find_first_not_of(del, pos);
   if (begin == string::npos) { tok = ""; return begin; }
   size_t end = str.find_first_of(del, begin);
   tok = str.substr(begin, end - begin);
   return end;
}

void splitString
(const string& option, vector<string>& tokens, const char* del)
{
   string token;
   size_t n = myStrGetTok(option, token, 0, del);
   while (token.size()) {
      tokens.push_back(token);
      n = myStrGetTok(option, token, n, del);
   }
}

void Line::clear()
{
    nick.clear();
    address.clear();
    cmd.clear();
    msg.clear();
    exp.clear();
}
