#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "./head.h"
void tcp_link()
{
    extern int sockfd;

    struct sockaddr_in saddr;

    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("socket");
		exit(1);
    }

    bzero(&saddr,sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT_NUM);
    saddr.sin_addr.s_addr = inet_addr("192.168.1.145");
    if(connect(sockfd,(struct sockaddr *)(&saddr),sizeof(struct sockaddr_in)) < 0)
    {
        perror("connect");
    }

    printf("connect success\n");

    
}


