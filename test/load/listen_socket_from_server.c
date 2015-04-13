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
#include "getuptime.h"

const char *ip="222.20.5.198";

int listenfd2;
void node_listensocket_fromserver()
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

    struct sockaddr_in client;
    socklen_t clien_addrlength=sizeof(client);
    char rbuf[200];
    char cont[30];
    float load=100.0;
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
                    printf("%s \n",rbuf);
                    if(rbuf[0]=='*'||rbuf[1]=='*'||rbuf[2]=='*'||rbuf[3]=='*'||rbuf[4]=='*'||rbuf[5]=='*'||rbuf[6]=='*')
                    {    
						load=av_load();
            			//printf("load %f \n",load);

            			strcpy(cont,ip);
            			memcpy(&cont[21],&load,4);
            			if(send(connfd, cont, sizeof(cont), 0 ) == -1 )
            			{
                			printf("send error \n");
                			close(connfd);
							break;
            			}
						//printf("send over \n");
					}
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
		node_listensocket_fromserver();
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
