#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void main()
{
   int port=8000;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   char buffer[255];

   server=gethostbyname("127.0.0.1");

   int sockfd=socket(AF_INET,SOCK_STREAM,0);

   bzero((char*)&serv_addr,sizeof(serv_addr));
   serv_addr.sin_family=AF_INET;
   serv_addr.sin_port=htons(port);
   bcopy((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);

   if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) <0)
   {
      printf("Error connecting to server");
      exit(1);
   }
   while(1)
   {
      bzero(buffer,255);
      fgets(buffer,255,stdin);
      write(sockfd,buffer,strlen(buffer));

      bzero(buffer,255);
      read(sockfd,buffer,255);
      printf("Server: %s\n",buffer);

      if(strncasecmp(buffer,"exit",4) ==0)
         break;
   }
   close(sockfd);
}
