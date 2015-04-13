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

#include"hashtable.h"
#include"ip.h"

#define N_SERVER  20 //同hashTH个数一样

int sendjob_conn[N_SERVER]={0};
int sendjob_sockfd[N_SERVER];
struct sockaddr_in sendjob_server_address[N_SERVER];


void send_befor(char *ip2,int id)
{
	if(ip2==NULL)
	{
		ip2=ip;
	}

    	bzero(&sendjob_server_address[id],sizeof(sendjob_server_address[id]));
   	sendjob_server_address[id].sin_family=AF_INET;   	
	inet_pton(AF_INET,ip2,&sendjob_server_address[id].sin_addr);
   	sendjob_server_address[id].sin_port=htons(1117);
re1:
		printf("id %d\n",id);
        sendjob_sockfd[id]=socket(PF_INET,SOCK_STREAM,0);
        if(sendjob_conn[id]=connect(sendjob_sockfd[id],(struct sockaddr*)&sendjob_server_address[id],sizeof(sendjob_server_address[id]))<0)
        {
            printf("11 connect error !!!\n");
	    	close(sendjob_sockfd[id]);
            goto re1;
        }

}

void sendjob_tonode(char *job,int id)
{
	

        if(sendjob_conn[id]<0)
        {
	re:
		//sleep(1);
		sendjob_sockfd[id]=socket(PF_INET,SOCK_STREAM,0);
		sendjob_conn[id]=connect(sendjob_sockfd[id],(struct sockaddr*)&sendjob_server_address[id],sizeof(sendjob_server_address[id]));	
        	if(sendjob_conn[id]<0)
		{	printf("22 connect error !!!\n");
					  close(sendjob_sockfd[id]);
            		goto re;
	        }
		}

        if(send(sendjob_sockfd[id], job, strlen(job)+1, 0 )< 0 )
        {
                //printf("send error \n");
                close(sendjob_sockfd[id]);
                goto re;
        }

        close(sendjob_sockfd[id]);
}



/*test...........*/
int main(int argc ,char *argv[])
{
	int i=0;
	for(i=0;i<argc-1;i++)
		send_befor(argv[i+1],0);
	sleep(1);
	printf("send...\n");
	char c[20]="0aaadfsdad";
	for(i=0;i<argc-1+10000;i++)
	{	
		printf("%s \n",c);
		sendjob_tonode(c,i%(argc-1));

		c[0]=((int)c[0]+(int)1);
		if(c[0]=='z') c[0]='0';
		usleep(1000);
		
	}
}


