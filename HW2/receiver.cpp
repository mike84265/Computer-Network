#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include "mySocket.h"
#include <assert.h>
#include <string.h>
#include <array>
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
   char buf[512];
   Data data;
   Buffer buffer;
   // Shake hand to establish connection with the agent.
   strcpy(buf, "Hello Server\n");
   client.write(buf,strlen(buf));
   assert(client.read(buf,sizeof(buf),-1) >= 0);
   while ( (nbytes = client.read((char*)&data, sizeof(data))) > 0) {
      if (data.num == -1) {
         // Final
         sprintf(data.buf,"ACK Final");
         client.write((char*)&data,sizeof(data));
         printf("recv\tFINAL\n");
         printf("send\tACK\tFINAL\n");
         int i;
         for (i=0;i<buffer.size()-1;++i)
            write(fd,buffer[i].buf,buffer[i].nbytes);
         return 0;
      }
      switch(buffer.push(data)) {
       case 0: // Normal
         sprintf(data.buf,"ACK %d", data.num);
         client.write((char*)&data,sizeof(data));
         printf("recv\tdata\t#%d\n",data.num);
         printf("send\tACK\t#%d\n",data.num);
         printf("size = %d\n",buffer.size());
         break;
       case -1: // Repeat PKG
         sprintf(data.buf,"ACK %d", data.num);
         client.write((char*)&data,sizeof(data));
         printf("ignr\tdata\t#%d\n",data.num);
         printf("send\tACK\t#%d\n",data.num);
         printf("size = %d\n",buffer.size());
         break;
       case -2: // Buffer overflow
         printf("drop\tdata\t#%d\n",data.num);
         if (buffer.full()) {
            for (int i=0;i<buffer.size();++i)
               write(fd,buffer[i].buf,PACKET_SIZE);
            buffer.clear();
            printf("flush\n");
         }
      }
   }
}
