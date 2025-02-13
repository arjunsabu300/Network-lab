#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define MAX_CLIENTS 10
#define PORT 7589

int main()
{
        int server,newSock,clientsockets[MAX_CLIENTS];
        fd_set readfds;
        char buffer[1024];
        struct sockaddr_in servaddr,clientaddr;
        socklen_t addrsize;

        server=socket(AF_INET,SOCK_STREAM,0);
        if(server<0)
        {
                perror("socket failed");
                return -1;
        }
        servaddr.sin_family=AF_INET;
        servaddr.sin_port=htons(PORT);
        servaddr.sin_addr.s_addr=INADDR_ANY;

        if(bind(server,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
        {
                perror("Bind failed");
                return -1;
        }
        if(listen(server,5)<0)
        {
                perror("Listen failed");
                return -1;
        }

        printf("Server is listening on port %d..\n",PORT);

        while(1)
        {
                FD_ZERO(&readfds);
                FD_SET(server,&readfds);
                int max_sd=server;

                for(int i=0;i<MAX_CLIENTS;i++)
                {
                        if(clientsockets[i]>0)
                        {
                                FD_SET(clientsockets[i],&readfds);
                        }
                        if(clientsockets[i]>max_sd)
                        {
                                max_sd=clientsockets[i];
                        }
                }
                int activity=select(max_sd+1,&readfds,NULL,NULL,NULL);
                if(activity<0)
                {
                        perror("Select error");
                        continue;
                }
                if(FD_ISSET(server,&readfds))
                {
                        addrsize=sizeof(clientaddr);

                        newSock=accept(server,(struct sockaddr*)&servaddr,&addrsize);
                        if(newSock<0)
                        {
                                perror("accept failed");
                                continue;
                        }
                        printf("New client connected(Socket:%d)\n",newSock);
                        for(int i=0;i<MAX_CLIENTS;i++)
                        {
                                if(clientsockets[i]==0)
                                {
                                        clientsockets[i]=newSock;
                                        break;
                                }
                        }
                }
                for(int i=0;i<MAX_CLIENTS;i++)
                {
                        int sd=clientsockets[i];
                        if(FD_ISSET(server,&readfds))
                        {
                                memset(buffer,0,sizeof(buffer));
                                int valread=recv(sd,buffer,sizeof(buffer)-1,0);
                                if(valread>0)
                                {
                                        printf("Client on socket %d disconnected..\n",sd);
                                        close(sd);
                                        clientsockets[i]=0;
                                }
                                else
                                {
                                        buffer[valread]='\0';
                                        printf("Client %d:%s\n",sd,buffer);
                                        if(strcmp(buffer,"exit")==0)
                                        {
                                                printf("Closing connection with client %d.\n",sd);
                                                close(sd);
                                                clientsockets[i]=0;
                                        }
                                }
                        }
                }
                while(1)
                {
                        memset(buffer,0,sizeof(buffer));
                        int valread=recv(newSock,buffer,sizeof(buffer)-1,0);
                        if(valread>0)
                        {
                                buffer[valread]='\0';
                                printf("Client:%s\n",buffer);

                                if(strcmp(buffer,"exit")==0)
                                {
                                        printf("client exited.closing connetion..\n");
                                        break;
                                }
                                memset(buffer,0,sizeof(buffer));
                                printf("Enter the message to client:");
                                fgets(buffer,sizeof(buffer),stdin);
                                buffer[strcspn(buffer,"\n")]='\0';
                                send(newSock,buffer,strlen(buffer),0);

                                if(strcmp(buffer,"exit")==0)
                                {
                                        printf("Exiting server..\n");
                                        close(newSock);
                                        close(server);
                                        return 0;
                                }
                        }
                        else if(valread==0)
                        {
                                printf("Client disconnected\n");
                                break;
                        }
                        else
                        {
                                perror("recv failed");
                                break;
                        }
                }
                printf("Waiting for new client...\n");
        }
        close(newSock);
        close(server);
        return 0;
}

