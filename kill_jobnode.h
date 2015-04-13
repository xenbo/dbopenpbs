/*************************************************************************
	> File Name: kill_jobnode
	> Author: DB
	> Mail: dongboyaojiayou@163.com
	> Created Time: Mon 13 Apr 2015 08:44:58 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "sendjob_tonode.h"

int find_minload_node()
{	int i=0,j=-1; int load=10000.0;
	for(i=0;i<20;i++)
	{
		//printf("node %d load %f \n",i,l_ser_node[i].load);
		if(l_ser_node[i].load>0&&l_ser_node[i].load<load)
		{
			j=l_ser_node[i].id;
			load=l_ser_node[i].load;
		}
	}	
	if(j>=0)
		return j;
		
	return -1;
} 

void *kill_jobnode()
{

    while(1)
    {
        struct jobnode *p=NULL;
        struct jobnode *q=JF;
        while(q)
        {
            if(q==JR)
            {
                sleep(1);
                break;
            }
            p=q;
            q=q->next;
            JF=q;
            printf("kill job  path %s  \n",p->path);//即发射到节点的任务，将计算的任务
            //find_minload_node()
	    //send job to node

		if(!pthread_mutex_trylock(&servernod_mutex))
        	{
			int id=0;
            		if((id=find_minload_node())>=0)
			{
				//printf("---> node   %d\n",id);
				//获取 id 节点的ip
								
				//sendsockt();				
			}
            		pthread_mutex_unlock(&servernod_mutex);
        	}		
			
            free(p);
            sleep(1);
        }
        sleep(2);
    }
}
