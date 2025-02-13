#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <stdlib.h>
int main()
{
        int serversocket;
        char buffer[1024];
        struct sockaddr_in serverAddr,clientaddr;
        socklen_t addrsize;

        serversocket=socket(AF_INET,SOCK_DGRAM,0);
        if(serversocket<0)
        {
                perror("Socket creation failed");
                return -1;
        }
        serverAddr.sin_family=AF_INET;
        serverAddr.sin_port=htons(6067);
        serverAddr.sin_addr.s_addr=inet_addr("127.0.0.75");


        if(bind(serversocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0)
        {
                perror("Bind failed");
                close(serversocket);
                return -1;
        }
        printf("server is listening...\n");
        addrsize=sizeof(clientaddr);
        while(1)
        {
                memset(buffer,0,sizeof(buffer));

                int recvLen=recvfrom(serversocket,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&clientaddr,&addrsize);
                if(recvLen>0)
                {
                        buffer[recvLen]='\0';
                        printf("Client:%s\n",buffer);
                        if(strcmp(buffer,"exit")==0)
                        {
                                printf("Client exited.\n");
                                continue;
                        }
                        memset(buffer,0,sizeof(buffer));
                        printf("Enter message : ");
                        fgets(buffer,sizeof(buffer),stdin);
                        buffer[strcspn(buffer,"\n")]='\0';
                        sendto(serversocket,buffer,strlen(buffer),0,(struct sockaddr*)&clientaddr,addrsize);
                        if(strcmp(buffer,"exit")==0)
                        {
                                printf("Server Shutting down...\n");
                                exit(1);
                        }

                }
        }
        close(serversocket);
        return 0;
}

