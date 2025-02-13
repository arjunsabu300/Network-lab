#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#define BUFFER_SIZE 1024

void prime(int num,char *response)
{
        if(num<2)
        {
                sprintf(response,"%d is neither prime nor composite",num);
                return;
        }
        int is_prime=1;
        char factors[BUFFER_SIZE] = "";
        int factor_count=0;

        for(int i=1;i<=num;i++)
        {
                if(num%i==0)
                {
                        is_prime=0;
                        char temp[20];
                        sprintf(temp," %d",i);
                        strcat(factors,temp);
                        factor_count++;
                }
        }
        if(factor_count==2)
        {
                sprintf(response," %d is a prime number",num);
        }
        else if(factor_count>2)
        {
                sprintf(response," %d is a composite number and its factors are:%s",num,factors);
        }
}

int main()
{
        int serversocket;
        char buffer[BUFFER_SIZE];
        struct sockaddr_in serverAddr,clientaddr;
        socklen_t addrsize;

        serversocket=socket(AF_INET,SOCK_DGRAM,0);
        if(serversocket<0)
        {
                perror("Socket creation failed");
                return -1;
        }
        serverAddr.sin_family=AF_INET;
        serverAddr.sin_port=htons(6265);
        serverAddr.sin_addr.s_addr=inet_addr("127.0.0.15");


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

                        if(strcmp(buffer,"exit")==0)
                        {
                                printf("Client exited.\n");
                                continue;
                        }
                        int num=atoi(buffer);
                        char response[BUFFER_SIZE];
                        prime(num,response);
                        printf("Server response:%s\n",response);
                        sendto(serversocket,response,strlen(response),0,(struct sockaddr*)&clientaddr,addrsize);

                        if(strcmp(buffer,"exit")==0)
                        {
                                printf("Server Shutting down...\n");
                                break;
                        }

                }
        }
        close(serversocket);
        return 0;
}

