//struct sockaddr_in addrMy;
//memset(&addrMy,0,sizeof(addrMy));
//int len = sizeof(addrMy);
//getsockname(sockfd,(struct sockaddr*)&addrMy,&len);
//printf("%d \n",ntohs(addrMy.sin_port));

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h> 
#define MaxLine 10000
#define Nt 2

struct jobnode
{
	int tid;
	char exename[30];
	char path[200];
	struct jobnode *next;
};

struct servernode
{
	int serid;
	int n_addjob;
	int staus;
	float load;
};
struct jobhead
{
	int id;
	int n_job_node;
	struct jobnode *fnext;
	struct jobnode *rnext;
	pthread_mutex_t tmutex;
};

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
struct jobhead JH[Nt];
pthread_t tid[Nt];
int listenfd;

char rbuf[MaxLine];
const char *ip="222.20.5.198";

int main()
{
    socklen_t len;


    struct sockaddr_in servaddr;
    listenfd =socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));

    servaddr.sin_family=AF_INET;
    inet_pton(PF_INET,ip,&servaddr.sin_addr);
    servaddr.sin_port=htons(1113);

    bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(listenfd,Nt);

  
	void thread_make(int*,int);
	void dojob();

	int i=0;
	for(i=0;i<Nt;i++)
	{		pthread_mutex_init(&(JH[i].tmutex),NULL);			
			JH[i].fnext=NULL;
			JH[i].rnext=NULL;
			JH[i].n_job_node=0;
	}
	
	int id[Nt];

	for(i=0;i<Nt;i++)
	{	
		id[i]=i;
		thread_make(&id[i],i);
	}
	sleep(1);
	
	printf("\n=======SERVER OKOK!!======\n");
	
	dojob();

	while(1)
		pause();
    return 0;
}


void thread_make(int *i,int j)
{
	void *thread_main(void *);
	pthread_create(&(tid[*i]),NULL,thread_main,(void*)i);
}

void *thread_main(void *arg)
{
	int connfd;
	int id=*(int *)arg;
	struct sockaddr_in client;
    socklen_t clien_addrlength=sizeof(client);
	

	while(1)
	{
		pthread_mutex_lock(&mutex);
		connfd=accept(listenfd,(struct sockaddr*)&client,&clien_addrlength);	
		pthread_mutex_unlock(&mutex);
		
		if(connfd<0)
			printf("listen error \n");

		int n=0;
    	while(1)
    	{
    		n = recv(connfd, rbuf, sizeof(rbuf), 0);
    		if(n>0)
    		{
   				rbuf[n] = '\0';
     /*			
				printf("tid %d recvmsg from client: %s\n", id,rbuf);
				n = send(connfd, "hello, this is server message!", strlen("hello, this is server message!"), 0 );
     			if(n<=0)
    			{
     				perror("sned error:");
    				close(connfd);
    				break;
    			}
	*/
    		}
    		else if(n <=0)
    		{
    	 		//printf("recv error:");
    			//close(connfd); 
    			break;   			
    		}    	
    	}
    	close(connfd);
		
		struct jobnode *jnode=(struct jobnode*)malloc(sizeof(struct jobnode));
		strncpy(jnode->path,rbuf,99);
		jnode->tid=id;
		jnode->next=NULL;

		pthread_mutex_lock(&JH[id].tmutex);
		if(!JH[id].fnext)
		{
				
 				JH[id].fnext=jnode;
				JH[id].rnext=jnode;
				JH[id].n_job_node++;
		}
		else
		{

			JH[id].rnext->next=jnode;
			JH[id].rnext=JH[id].rnext->next;
			JH[id].n_job_node++;
	
			//printf(" tid %d sum_job %d \n",id,JH[id].n_job_node);
		}
		pthread_mutex_unlock(&JH[id].tmutex);
	
	}
	pthread_exit(0);
	//close(connfd);
}

void dojob()
{
	void* thread_sendjob();
	pthread_t tdsjob;
	pthread_create(&(tdsjob),NULL,thread_sendjob,NULL);
}

void *thread_sendjob()
{	
	int i=0;
	for(;;)
	{
			if(!pthread_mutex_trylock(&JH[i].tmutex))
			{
			//	printf("lock list %d \n",i);

 				struct jobnode *p=NULL;//JH[i].fnext->next;
				struct jobnode *q=JH[i].fnext;
				while(q)
				{	
 					p=q;
					printf(" free tid %d  path %s  \n",p->tid,p->path);
					q=q->next;
					JH[i].fnext=q;
					free(p);
				}

				JH[i].fnext=NULL;
				JH[i].rnext=NULL;
				JH[i].n_job_node=0;
					
				pthread_mutex_unlock(&JH[i].tmutex);
				
			}
			else
			{
				printf(" no get lock \n");
			}
			i++;
			if(i==Nt)
				i=0;
	}
}


