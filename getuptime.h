/*************************************************************************
	> File Name: getuptime.c
	> Author: ma6174
	> Mail: ma6174@163.com
	> Created Time: 2015年04月12日 星期日 14时20分00秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include<stdlib.h>

float av_load()
{

    int fd[2];
    pipe(fd);

    pid_t pid=fork();
    if(pid<0)
        printf("pid error \n");
    if(pid==0)
    {
        dup2(fd[1], STDOUT_FILENO);
        execv("/usr/bin/uptime",NULL);
        exit(0);
    }

    char mem[100];
    usleep(1000);
    int n=read(fd[0],mem,100);


    wait(NULL);

    float load=100.0,load2=100.0;
    char s1[100];

    //printf("%s\n",mem);

    sscanf(&mem[46],"%4f",&load);
    sscanf(&mem[51],"%4f",&load2);
    //printf("%f %f\n",load,load2);

    return (load+load2)/2;

}

