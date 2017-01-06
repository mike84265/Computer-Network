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
   int fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
   while ( (nbytes = client.read((char*)&data, sizeof(data))) > 0) {
      write(fd, data.buf, nbytes-sizeof(int));
      sprintf(data.buf,"ACK %d", data.num);
      client.write((char*)&data,sizeof(data));
      printf("recv\tdata\t#%d\n",data.num);
      printf("send\tACK\t#%d\n",data.num);
   }
}
