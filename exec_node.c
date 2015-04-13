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

#include "getuptime.h"

#define MaxLine 1000

int listenfd;
char rbuf[MaxLine];

#include"ip.h"


int send_load()
{
    pid_t pid;

rebootchild:
    if((pid=fork())==0)
    {

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

                float load=av_load();
                //printf("load %f \n",load);
                char cont[30];
                strcpy(cont,ip);
                memcpy(&cont[21],&load,4);
                if(send(sockfd, cont, sizeof(cont), 0 ) == -1 )
                {
                    printf("send error \n");
                    //close(sockfd);
                    goto re;
                }
                sleep(3);
            }

            close(sockfd);
        }
        exit(0);
    }
    else if(pid<0)
    {
        printf("pid error ....\n");
    }

    sleep(1);

    int pidstat;
    if((pidstat=waitpid(pid,NULL,0))<0)
        printf("error...\n");
    else if(pidstat>0)
    {
        goto rebootchild;
    }
}

int main()
{
    send_load();
}


