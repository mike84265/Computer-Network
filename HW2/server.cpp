#include <cstdio>
#include <iostream>
#include <pthread.h>
#include "mySocket.h"
#include "rnGen.h"
#define ERR_EXIT(str)      \
   { cerr << str;  exit(1);  }
using namespace std;
myServer sndServer;
myServer rcvServer;
int numsndpkg, numloss;
void* send_fwd(void* arg)
{
   int nbytes;
   Data data;
   rnGen rn; 
   while ( (nbytes = sndServer.read((char*)&data,sizeof(data))) > 0) {
      printf("get\tdata\t#%d\n",data.num);
      ++numsndpkg;
      if (rn(50) == 0) {
         ++numloss;
         printf("drop\tdata\t#%d\tloss rate = %.4f\n",data.num, (double)numloss/numsndpkg);   
      } else {
         rcvServer.write((char*)&data,nbytes);
         printf("fwd\tdata\t#%d\tloss rate = %.4f\n",data.num, (double)numloss/numsndpkg);
      }
   }
   return NULL;
}

void* rcv_fwd(void* arg)
{
   int nbytes;
   Data data;
   while ( (nbytes = rcvServer.read((char*)&data,sizeof(data))) > 0) {
      printf("get\tACK\t#%d\n",data.num);
      printf("fwd\tACK\t#%d\n",data.num);
      sndServer.write((char*)&data,nbytes);
   }
   return NULL;
}
int main(int argc, char** argv)
{
   if (argc < 3) {
      fprintf(stderr,"Usage: %s <sndport> <rcvport>\n",argv[0]);
      exit(1);
   }
   sndServer.init(argv[1]);
   rcvServer.init(argv[2]);
   char buf[128];
   int nbytes;

   if ( (nbytes = sndServer.read(buf,sizeof(buf),-1)) >= 0) {
      printf("Sender side is ready.\n");
   } else
      ERR_EXIT("sndServer read error!\n")
   if ( (nbytes = rcvServer.read(buf,sizeof(buf),-1)) >= 0) {
      printf("Receiver side is ready.\n");
   } else
      ERR_EXIT("rcvServer read errror!\n")

   // Both side are ready. Sending message to tell them.
   sndServer.write(buf,nbytes);
   rcvServer.write(buf,nbytes);

   pthread_t sndtid, rcvtid;
   pthread_create(&sndtid, NULL, send_fwd, NULL);
   pthread_create(&rcvtid, NULL, rcv_fwd, NULL);
   
   pthread_join(sndtid,NULL);
   pthread_join(rcvtid,NULL);

   rcvServer.write(buf,0);
}

