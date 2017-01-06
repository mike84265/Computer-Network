#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include "mySocket.h"
#include <assert.h>
#include <fcntl.h>
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
   char buf[512];
   int threshold=16, winSize=1, oldestPKG=0;
   Data data;
   // Shake hand to establish connection with the agent.
   strcpy(buf, "Hello Server\n");
   client.write(buf,strlen(buf));
   assert(client.read(buf,sizeof(buf),-1) >= 0);
   int num = 0, i=0, final=0;
   while(1) {
      for (i=0;i<winSize;++i) {
         nbytes = read(fd, data.buf, sizeof(data.buf));
         data.nbytes = nbytes;
         data.num = num++; 
         client.write((char*)&data, nbytes + sizeof(int));
         printf("send\tdata\t#%d\twinSize=%d\n",data.num,winSize);
         if (nbytes <= 0) {
            winSize = i+1;
            printf("final, winSize changed to %d\n",winSize);
            final = 1;
            break;
         }
      }
      bool loss = false;
      for (i=0;i<winSize;++i) {
         if ( (nbytes = client.read((char*)&data,sizeof(data),0.5)) > 0) {
            printf("recv\tACK\t#%d\n", data.num);
            if (data.num == oldestPKG ) {
               // Normal transmission
               oldestPKG = data.num + 1;
            } else {
               // Package lost
               loss = true;
            }
         } else {
            // No response from receiver.
            loss = true;
            break;
         }
      }
      if (!loss) {
         winSize = (winSize >= threshold)? winSize+1 : winSize*2;
         if (final)
            break;
      } else {
         threshold = (winSize > 1)? (winSize / 2) : 1;
         winSize = 1;
         lseek(fd,oldestPKG * PACKET_SIZE,SEEK_SET);
         num = oldestPKG;
         printf("time\tout,\t\tthreshold = %d\n",threshold);
      }
   }
   data.num = -1;
   strcpy(data.buf,"FINAL");
   client.write((char*)&data,sizeof(data));
   client.read((char*)&data,sizeof(data),-1);
   printf("recv\tACK\tFINAL\n");
   client.write(buf,0);
}
