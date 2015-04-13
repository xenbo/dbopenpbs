/*************************************************************************
	> File Name: hashtable.c
	> Author: DB
	> Mail: dongboyaojiayou@163.com
	> Created Time: Mon 13 Apr 2015 08:44:58 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void heap_sort(int st,int maxed)
{
    int i=st;
    int j=2*i;
    float tm=l_ser_node[i].load;
    while(j<maxed)
    {
        if(j<maxed && l_ser_node[j].load>l_ser_node[j+1].load)
            j++;

        if(l_ser_node[j].load<tm)
        {
            l_ser_node[i].load=l_ser_node[j].load;
            i=j;
            j=2*i;
        }
        else break;
    }
    l_ser_node[i].load=tm;
}

void sort_load()
{
    int n=N_SERVER;
    int i,j,k;
    for(i=n/2; i>=0; i--)
    {
        heap_sort(i,n);
    }
}

int get_minloda_node()
{
    heap_sort(0,N_SERVER);
    return l_ser_node[0].id;
}


