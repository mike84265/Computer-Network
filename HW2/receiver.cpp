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
   int fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
   if (fd < 0) {
      fprintf(stderr, "file error\n");
      exit(1);
   }

   int nbytes;
   char buf[BUFSIZE];
   strcpy(buf, "Hello Server\n");
   client.write(buf,strlen(buf));
   assert(client.read(buf,sizeof(buf)) >= 0);
   while ( (nbytes = client.read(buf, sizeof(buf))) > 0) {
      write(fd, buf, nbytes);
      printf("Receiving %d bytes from agent\n", nbytes);
   }
}
