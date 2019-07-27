#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sqlite3.h>
#include "./head.h"

void *pthread_scan(void *arg)
{
    while(1)
    {
        client_mannage();
    }
}

void *pthread_handle(void *arg)
{
    extern sqlite3 *db;
    extern Link online_user;
    struct send_pack recv_pack;
	struct current_data temp;//因为一个线程对应一个客户端，在接收客户端发送的登陆请求之后，将用户的id，name，存在属于这个线程的一个结构体中
	char buf[MAX_SIZE] = {0};
	
	int fd = *((int *)arg);
	
	memset(&temp,0,sizeof(temp));
    
    while(1)
    {
        memset(&recv_pack,0,sizeof(recv_pack));
        recv_pk(&recv_pack,fd);
        
	    if(recv_pack.send_cmd != 0)
	    {
	    	printf("# Recive cmd %d from cfd %d\n",recv_pack.send_cmd,fd);
	    }
	    
        switch(recv_pack.send_cmd)
        {
            case SIGNUP:
			{
				handle_signup(recv_pack,fd);
		        break;
			}
			
	        case LOGIN:
			{
				handle_login(recv_pack,fd,&temp);
		        break;
			}
			
	        case CLIENT_EXIT:
			{
				printf("# Client %d offline\n",fd);		          
	            pthread_exit(NULL);
		        break;
			}
			
	        case MUTI_SEND:
			{
				handle_send(recv_pack,fd,temp);
		        break;
			}
			
	        case REFIND:
			{
				handle_refind(recv_pack,fd);
		        break;
			}
			
		    case OFFLINE:
			{
				printf("# The user whose id is %d has gone offline\n",temp.current_id1);
				sprintf(buf,"%d",temp.current_id1);
				remove_node(online_user,buf,1);
		    	break;
			}
		    
			case FRIEND_REQ:
			{
				handle_add_friend(recv_pack,fd,temp);
				break;
			}
			
			case FRIEND_ACC:
			{
				handle_accept_friend(recv_pack,fd,temp);
				break;
			}
			
			case GROUP:
			{
				handle_group(fd,temp);
				break;
			}
			
			case CHANGE:
			{
				handle_change(recv_pack,fd,&temp);
				break;
			}
			
			case SHOW:
			{
				handle_show(temp,recv_pack,fd);
				break;
			}

			case TAN_FILE:
			{
				handle_file(fd,recv_pack.send_msg0,temp);
				break;
			}

			case CHECK:
			{
				handle_message(fd,recv_pack,temp);
				break;
			}

			case FRESH:
			{
				handle_fresh(fd,temp);
				break;
			}

			case DELETE:
			{
				//handle_delete(fd,recv_pack,temp);
				break;
			}
        }
    }
	
}
void server_connect()
{
    extern int sockfd;
    struct sockaddr_in saddr,caddr;
    pthread_t id_scan,id_handle;
    int cfd;
    int opt = 1;
	
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        perror("socket");
     	exit(1);
    }

    printf("# This server is running , sockfd is %d\n",sockfd);
    sleep(1);
	system("clear");
	
    
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    bzero(&saddr,sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT_NUM);
    saddr.sin_addr.s_addr = inet_addr("192.168.1.145");

    if ((bind(sockfd,(struct sockaddr *)(&saddr),sizeof(struct sockaddr_in))) < 0)
    {
        perror("bind");
	    exit(1);
    }

    if(listen(sockfd,3) < 0)
    {
        perror("listen");
    }
    
    if(pthread_create(&id_scan,NULL,pthread_scan,NULL) != 0)
    {
    	perror("pthread");
    	exit(1);
    }
    while(1)
    {
		bzero(&caddr,sizeof(struct sockaddr_in));
		int clen;
		clen = sizeof(struct sockaddr_in);
		if ((cfd = accept(sockfd,(struct sockaddr *)(&caddr),&clen)) < 0)
		{
		    perror("accept");
		    exit(1);
		}
    
        printf("# Connect with cfd %d successfully\n",cfd);
        
      	if(pthread_create(&id_handle,NULL,pthread_handle,(void *)(&cfd)) != 0)
		{
		    perror("pthread");
		    exit(1);
		}
    }
}
