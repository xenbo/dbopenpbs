/*************************************************************************
	> File Name: listen_exec_node.h
	> Author: ma6174
	> Mail: ma6174@163.com
	> Created Time: 2015年04月12日 星期日 16时07分37秒
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
void *listen_exec_node()
{


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
                float load;
                memcpy(&load,&rbuf[21],4);
                printf("%s load %f\n",rbuf,load);

            }
            else if(n <=0)
            {
                //printf("recv error:");
                //close(connfd);
                break;
            }
        }
        close(connfd);
    }

    pthread_exit(0);
}

void thread_listen_node()
{

    socklen_t len;
    struct sockaddr_in servaddr;
    listenfd2 =socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));

    servaddr.sin_family=AF_INET;
    inet_pton(PF_INET,ip,&servaddr.sin_addr);
    servaddr.sin_port=htons(1115);

    bind(listenfd2,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(listenfd2,5);

    sleep(2);

    pthread_t tid;
    pthread_create(&(tid),NULL,listen_exec_node,NULL);

    while(1)
        pause();
}
int main()
{
    thread_listen_node();
}
