#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include "mySocket.h"
#include <assert.h>
#include <string.h>
using namespace std;
#define BUFSIZE 128
int main(int argc, char** argv)
{
   if (argc < 4) {
      fprintf(stderr,"Usage: %s hostname port filename", argv[0]);
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
   strcpy(buf, "Hello Server\n");
   client.write(buf,strlen(buf));
   assert(client.read(buf,sizeof(buf)) >= 0);
   while ( (nbytes = read(fd, buf, sizeof(buf))) > 0) {
      client.write(buf, nbytes);
      printf("Sending %d bytes to agent\n", nbytes);
      client.read(buf,sizeof(buf));
      printf("Getting %s from server\n", buf);
   }
   client.write(buf,0);
}
