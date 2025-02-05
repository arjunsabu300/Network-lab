#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void main()
{
   int port=8000;
   struct sockaddr_in serv_addr,cli_addr;
   socklen_t clilen;
   char buffer[255];

   bzero((char*)&serv_addr,sizeof(serv_addr));
   serv_addr.sin_family=AF_INET;
   serv_addr.sin_addr.s_addr=INADDR_ANY;
   serv_addr.sin_port=htons(port);

   int sockfd=socket(AF_INET,SOCK_STREAM,0);
   if(sockfd <0)
      printf("Error opening socket\n");

   bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
   listen(sockfd,5);
   clilen=sizeof(cli_addr);
   int newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clilen);

   while(1)
   {
      bzero(buffer,255);
      read(newsockfd,buffer,25);
      printf("Client: %s\n",buffer);
      bzero(buffer,255);
      fgets(buffer,255,stdin);
      write(newsockfd,buffer,strlen(buffer));

      if(strncasecmp(buffer,"exit",4) ==0)
         break;
   }
   close(newsockfd);
   close(sockfd);
}
