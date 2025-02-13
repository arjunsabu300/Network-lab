#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
        int clientsocket;
        char buffer[1024];
        struct sockaddr_in serverAddr;
        socklen_t addrsize;

        clientsocket=socket(AF_INET,SOCK_DGRAM,0);
        if(clientsocket<0)
        {
                perror("Socket creation failed");
                return -1;
        }
        serverAddr.sin_family=AF_INET;
        serverAddr.sin_port=htons(6265);
        serverAddr.sin_addr.s_addr=inet_addr("127.0.0.15");

        printf("Connected to server.\n");
        while(1)
        {
                memset(buffer,0,sizeof(buffer));
                printf("Enter a number: ");
                fgets(buffer,sizeof(buffer),stdin);
                buffer[strcspn(buffer,"\n")]='\0';
                sendto(clientsocket,buffer,strlen(buffer),0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

                if(strcmp(buffer,"exit")==0)
                {
                        printf("Exiting client...\n");
                        break;
                }
                addrsize=sizeof(serverAddr);
                int recvLen=recvfrom(clientsocket,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&serverAddr,&addrsize);
                if(recvLen>0)
                {
                        buffer[recvLen]='\0';
                        printf("Server response:%s\n",buffer);
                        if(strcmp(buffer,"exit")==0)
                        {
                                printf("Server has exited.closing client...\n");
                                break;
                        }
                }
                else
                {
                        perror("No response from server");
                        printf("Server might have exited.Closing client..\n");
                        break;
                }

        }
        close(clientsocket);
        return 0;
}


