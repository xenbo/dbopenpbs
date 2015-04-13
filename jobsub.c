#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<unistd.h>
#include <string.h>
#include<arpa/inet.h>


#define MaxLine 10000

#include"ip.h"

int main()
{


    char rbuf[MaxLine];
    struct sockaddr_in server_address;
    bzero(&server_address,sizeof(server_address));
    server_address.sin_family=AF_INET;
    inet_pton(AF_INET,ip,&server_address.sin_addr);
    server_address.sin_port=htons(1113);


    /*
    	struct timeval timeo;
    	timeo.tv_sec  = 0;
    	timeo.tv_usec = 1000 * 1000;
    	socklen_t len = sizeof(timeo);
    	setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeo, len);
    */
    int i=-1;

    int n=0;


    //while(1)
    {
        int sockfd;
re:
        //sleep(0.01);
        sockfd=socket(PF_INET,SOCK_STREAM,0);
        if(connect(sockfd,(struct sockaddr*)&server_address,sizeof(server_address))<0)
        {
            printf("connect error !!!\n");
            goto re;
        }

        if(send(sockfd, "hello, this is client message!", strlen("hello, this is client message!"), 0 ) == -1 )
        {
            printf("send error \n");
            goto re;
        }

        /*
        while(1)
        {
        	n = recv(sockfd, rbuf, 200, 0);
        	if(n>0)
        	{
        		rbuf[n] = '\0';
        		printf("recvmsg from client: %s\n", rbuf);
        	}
        	else if(n <=0)
        	{
        		printf("recv error:");
        		//close(sockfd);
        		break;
        	}
        	if(send(sockfd, "hello, this is client message!", strlen("hello, this is client message!"), 0 ) == -1 )
        	{
        		printf("send error \n");
        		goto re;
        	}

        	sleep(1);
        }
        */
        close(sockfd);
    }
    //close(sockfd);
}
