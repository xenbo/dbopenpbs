/*************************************************************************
	> File Name: exec_node.c
	> Author: ma6174
	> Mail: ma6174@163.com
	> Created Time: 2015年04月12日 星期日 14时02分04秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<unistd.h>
#include <string.h>
#include<arpa/inet.h>


#define MaxLine 1000

const char *ip="222.20.5.198";

void *getload_from_node()
{
		
		int listenfd;
		char rbuf[MaxLine];
        struct sockaddr_in server_address;
        bzero(&server_address,sizeof(server_address));
        server_address.sin_family=AF_INET;
        inet_pton(AF_INET,ip,&server_address.sin_addr);
        server_address.sin_port=htons(1115);
        int sockfd;

        while(1)
        {

		re:
            sleep(1);
            sockfd=socket(PF_INET,SOCK_STREAM,0);
            if(connect(sockfd,(struct sockaddr*)&server_address,sizeof(server_address))<0)
            {
                printf("connect error !!!\n");
                goto re;
            }

            while(1)
            {
                if(send(sockfd, "*************", strlen("*************")+1, 0 ) == -1 )
                {
                    printf("send error \n");
                    close(sockfd);
                    goto re;
                }

                int n=0;
                while(1)
                {
                    n = recv(sockfd, rbuf, sizeof(rbuf)+1, 0);
                    if(n>0)
                    {
                        float load;
                        memcpy(&load,&rbuf[21],4);
                        printf("%s load %f\n",rbuf,load);
						break;
                    }
                    else if(n <=0)
                    {
                        printf("recv error:");
                        close(sockfd);
                        break;
                    }
                }
      			printf("server send agein....\n");
                sleep(3);
            }

            close(sockfd);
        }
}



int main()
{
	getload_from_node();
}
/*
*/


