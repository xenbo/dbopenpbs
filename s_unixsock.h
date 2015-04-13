#define MAX_SEND 1025
#define UNIX_PATH "/tmp/sinfor"
int unixconn_sock;
void s_unixsocket(char *mess)
{
    int n=write(unixconn_sock, "hello, this is client message!", strlen("hello, this is client message!")+1 );
    if(n< 0 )
    {
        printf("send error \n");
    }
}

int unixsocket()
{
    unixconn_sock= socket(AF_LOCAL, SOCK_STREAM, 0);
    if(unixconn_sock== -1)
    {
        perror("socket fail ");
        return -1;
    }
    struct sockaddr_un addr;
    bzero(&addr, sizeof(addr));
    addr.sun_family = AF_LOCAL;
    strcpy((void*)&addr.sun_path, UNIX_PATH);
    if(connect(unixconn_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("connect fail ");
        return -1;
    }
    return 0;
}
