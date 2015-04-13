/*************************************************************************
	> File Name: revjob_insertlist
	> Author: DB
	> Mail: dongboyaojiayou@163.com
	> Created Time: Mon 13 Apr 2015 08:44:58 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void *revjob_insertlist()
{
    int unixsockfd;
    unixsockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(unixsockfd == -1)
    {
        perror("socket func fail ");
        return ;
    }
    struct sockaddr_un ser_addr, cli_addr;
    ser_addr.sun_family = AF_LOCAL;
    strcpy(ser_addr.sun_path, UNIX_SERV_PATH);
    unlink(UNIX_SERV_PATH);
    bind(unixsockfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    listen(unixsockfd, 20);

    //int c=0;
    //int c2=0;
    //char cm[10000][100];

    char rc[200];

    while(1)
    {

        sleep(1);
        printf("job ....\n ");

        int len = sizeof(cli_addr);
        int connfd = accept(unixsockfd, (struct sockaddr*)&cli_addr, &len);



        //fcntl(connfd,  F_SETFL, O_NONBLOCK );
        if(connfd <0)
        {
            printf("error \n");
            break;
        };

        int n=0;
        while(1)
        {
            //bzero(rbuf,MAX_RECV);
            n = read(connfd, rc, 150);
            if(n>0)
            {
                //cm[c][n] = '\0';
                //printf("%d   ",c2);
                //printf(" %d %s \n",c,cm[c]);
                //c++;

                struct jobnode *tmp=(struct jobnode*)malloc(sizeof(struct jobnode));
                tmp->next=NULL;
                strcpy(tmp->path,rc);
                if(JF==JR)
                {
                    JF=tmp;
                    tmp->next=JR;
                    JR->next=tmp;
                    n_job++;
                } else
                {
                    JR->next->next=tmp;
                    tmp->next=JR;
                    JR->next=tmp;
                    n_job++;
                }
                printf("new job  path %s %d\n",tmp->path,n_job);//新增加的任务。。。
            }
            else if(n <=0)
            {
                printf("connect agin... \n");
                break;
            }
            //if(c==10000)
            //{	c=0;c2++;}
        }

        close(connfd);
    }
    close(unixsockfd);
    /*
    for(c=0;c<10000;c++)
    {
    	printf("%s \n",cm[c]);
    }
    */
}

