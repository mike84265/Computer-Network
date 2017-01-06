#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include "mySocket.h"
#include <assert.h>
#include <string.h>
using namespace std;
int main(int argc, char** argv)
{
   if (argc < 4) {
      fprintf(stderr,"Usage: %s <hostname> <port> <filename>", argv[0]);
      exit(1);
   }

   myClient client(argv[1], argv[2]);
   int fd = open(argv[3], O_RDONLY);
   if (fd < 0) {
      fprintf(stderr, "file error\n");
      exit(1);
   }

   int nbytes;
   char buf[BUFSIZE];
   Data data;
   strcpy(buf, "Hello Server\n");
   client.write(buf,strlen(buf));
   assert(client.read(buf,sizeof(buf)) >= 0);
   int num = 0;
   while ( (nbytes = read(fd, data.buf, sizeof(data.buf))) > 0) {
      data.num = ++num; 
      client.write((char*)&data, nbytes + sizeof(int));
      printf("send\tdata\t#%d\n", num);
      client.read((char*)&data,sizeof(data));
      printf("recv\tACK\t#%d\n", data.num);
   }
   client.write(buf,0);
}
