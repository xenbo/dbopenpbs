/*************************************************************************
	> File Name: cli_compute.c
	> Author: DB
	> Mail: dongboyaojiayou@163.com 
	> Created Time: Mon 13 Apr 2015 01:57:38 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/un.h>
#include <errno.h>

#include"ip.h"

int listenfd2;
void getjob_fromserver()
{
    socklen_t len;
    struct sockaddr_in servaddr;
    listenfd2 =socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));

    servaddr.sin_family=AF_INET;
    inet_pton(PF_INET,ip,&servaddr.sin_addr);
    servaddr.sin_port=htons(1117);

    bind(listenfd2,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(listenfd2,25);


    struct sockaddr_in client;
    socklen_t clien_addrlength=sizeof(client);
    char rbuf[200];

    while(1)
    {
        int connfd;
        connfd=accept(listenfd2,(struct sockaddr*)&client,&clien_addrlength);
        if(connfd<0)
        {
            printf("accept  error ....\n");
        }

        int n=0;

        while(1)
        {
            n = recv(connfd, rbuf, sizeof(rbuf)+1, 0);
            if(n>0)
            {
		rbuf[n]='\0';
                printf("%s \n",rbuf);//get job content;
               	break;
            }
            else if(n <=0)
            {
                //printf("recv error:");
                close(connfd);
                break;
            }
        }
        close(connfd);
    }
    exit(0);
}

void pid_listen()
{
    pid_t pid;
rebootchild:

    if((pid=fork())==0)
    {
        getjob_fromserver();
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
    pid_listen();
}
