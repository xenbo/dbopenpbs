#include <sys/un.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h>

#define UNIX_SERV_PATH "/tmp/sinfor"
#define N_SERVER  20 //同hashTH个数一样

struct servernode_con
{
    int id;
    int n_addjob;
    int staus;
    char ip[20];
    float load;
};
struct servernode_con l_ser_node[N_SERVER];



struct jobnode
{
    int id;
    char path[150];
    struct jobnode *next;
};

pthread_mutex_t servernod_mutex=PTHREAD_MUTEX_INITIALIZER;

int n_job=0;
struct jobnode *JF=NULL;
struct jobnode *JR=NULL;

#include "get_load_from_node.h"
#include "kill_jobnode.h"
#include "revjob_insertlist.h"


int main(int argc, char** argv)
{

    memset(l_ser_node,0,sizeof(l_ser_node));
    JR=(struct jobnode *)malloc(sizeof(struct jobnode));
    JF=JR;
    JR->next=JF;

    pthread_t tid1,tid2,tid3;

    /// 1
    pthread_create(&tid1,NULL,revjob_insertlist,NULL);

    /// 2
    pthread_create(&tid2,NULL,kill_jobnode,NULL);

    /// 3
    pthread_create(&tid3,NULL,getload_from_node,NULL);


    sleep(1);
    printf("\n===== list OK====== \n");

    while(1)
        pause();
    return 0;
}


