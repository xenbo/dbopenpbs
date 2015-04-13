/*************************************************************************
	> File Name: hashtable.c
	> Author: DB
	> Mail: dongboyaojiayou@163.com
	> Created Time: Mon 13 Apr 2015 08:44:58 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define Nt 20
struct hashtable
{
    int id;
    char ip[25];
    int cout;
};


struct hashtable HTable[Nt];
int sn=0;
//二次探测法
int  hashinsert(char *k,int p,int m)
{
    int i,addr;
    int n=0;
    memcpy(&n,&k[strlen(k)-4],4);
    addr=n%p;
    if(HTable[addr].ip[0]=='\0'||HTable[addr].ip[0]=='*')
    {
        strcpy(HTable[addr].ip,k);
        HTable[addr].cout=1;
        HTable[addr].id=addr;
        sn++;
	return addr;
    }
    else
    {
        i=1;
        do
        {
            addr=(addr+1)%m;
            i++;

        } while(HTable[addr].ip[0]!='\0' && HTable[addr].ip[0]!='*');
        strcpy(HTable[addr].ip,k);
        HTable[addr].cout=i;
        HTable[addr].id=addr;
        sn++;

	return addr;
    }
    return -1;
    printf("%d  %d \n",HTable[addr].cout,addr);

}

int seachHT(char *k,int p,int m)
{
    int i,addr;
    int n=0;
    memcpy(&n,&k[strlen(k)-4],4);
    addr=n%p;

    while(strcmp(HTable[addr].ip,k)!=0)
    {
        addr=(addr+1)%m;
    }

    if(strcmp(HTable[addr].ip,k)==0)
        return addr;
    return -1;
}

int deleteHT(char *k,int p,int m)
{
    int addr=seachHT(k,p,m);
    if(addr>=0)
    {   HTable[addr].ip[0]='*';
        sn--;
        return 1;
    }
    return 0;
}

/*

//test...............
char c[20][25]={
	"10.1.1.1",
	"10.1.1.2",
	"10.1.1.3",
	"10.1.1.4",
	"10.1.1.5",
	"10.1.1.6",
	"10.1.1.7",
	"10.1.1.8",
	"10.1.1.9",
	"10.1.1.10"
	"10.1.1.11",
	"10.1.1.12",
	"10.1.1.13",
	"10.1.1.14",
	"10.1.1.15",
	"10.1.1.16",
	"10.1.1.17",
	"10.1.1.18",
	"10.1.1.19",
	"10.1.1.20"
	};
int main()
{
	int i,j;
	memset(HTable,0,sizeof(struct hashtable)*hlong);
	for(i=0;i<hlong;i++)
		hashinsert(c[i],20,23);
	printf("=======================================\n");
	for(i=0;i<hlong;i++)
		printf("%d %d\n",seachHT(c[i],20,23),HTable[i].id);
	printf("=======================================\n");
	for(i=0;i<hlong;i++)
		printf("%d\n",deleteHT(c[i],20,23));
}
*/
