#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "./head.h"
#include <sqlite3.h>

int send_pk(struct send_pack *pk,int sockfd)
{
    
    if(send(sockfd,pk,sizeof(*pk),0) < 0)
    {
		perror("send");
		exit(1);
		return FAIL;
    }
    return SUCCESS;
}

int recv_pk(struct send_pack *pk,int sockfd)
{
    
    if(recv(sockfd,pk,sizeof(*pk),0) < 0)
    {
    	perror("recv");
    	exit(1);
		return FAIL;
    }
    return SUCCESS;
}

int handle_show(struct current_data temp,struct send_pack recv,int cfd)
{
	extern Link cb_head;
	extern sqlite3 *db;
	extern Link online_user;
	extern char temp_name[MAX_SIZE];
	struct send_pack msg_pack;
	char buf[MAX_SIZE] = {0};
	Link p;
	int count = 0;

	memset(&msg_pack,0,sizeof(msg_pack));
	
	switch(recv.result)
	{
		case 1://signle of all friend
		{
			sprintf(msg_pack.send_msg0,"\n|%-10s|%-10s|","id","name");
			msg_pack.send_cmd = INFO;
			send_pk(&msg_pack,cfd);
			search_data(db,temp.current_name,"dec","friend",INPORT);
			p = cb_head->next;
			while(p != NULL)
			{
				memset(&msg_pack,0,sizeof(msg_pack));
				sprintf(msg_pack.send_msg0,"|%-10d|%-10s|",p->id1,p->message);
				msg_pack.send_cmd = INFO;
				send_pk(&msg_pack,cfd);
				p = p->next;
			}
			break;
		}

		case 2:
		{
			sprintf(msg_pack.send_msg0,"\n|%-10s|%-10s|","id","name");
			msg_pack.send_cmd = INFO;
			send_pk(&msg_pack,cfd);
			p = online_user->next;
			while(p != NULL)
			{
				memset(&msg_pack,0,sizeof(msg_pack));
				sprintf(msg_pack.send_msg0,"|%-10d|%-10s|",p->id1,p->name);
				msg_pack.send_cmd = INFO;
				send_pk(&msg_pack,cfd);
				p = p->next;
			}
			break;
		}

		case 3:
		{
			count = search_data(db,"userdata",NULL,NULL,INPORT);
			p = cb_head->next;
			sprintf(msg_pack.send_msg0,"\n|%-10s|%-10s|","id","name");
			msg_pack.send_cmd = INFO;
			send_pk(&msg_pack,cfd);
			while(p != NULL)
			{
				memset(&msg_pack,0,sizeof(msg_pack));
				sprintf(msg_pack.send_msg0,"|%-10d|%-10s|",p->id1,p->name);
				msg_pack.send_cmd = INFO;
				send_pk(&msg_pack,cfd);
				p = p->next;
			}
			break;
		}

		case 4:
		{
			search_by_id(db,"friendreq","id2",temp.current_id1,INPORT);
			sprintf(msg_pack.send_msg0,"\n|%-10s|%-10s|","id","name");
			msg_pack.send_cmd = INFO;
			send_pk(&msg_pack,cfd);
			p = cb_head->next;
			while(p != NULL)
			{
				search_by_id(db,"userdata","id1",p->id1,EXCHANGE);
				sprintf(msg_pack.send_msg0,"|%-10d|%-10s|",p->id1,temp_name);
				msg_pack.send_cmd = INFO;
				send_pk(&msg_pack,cfd);
				p = p->next;
			}
			break;
		}

		case 5:
		{

		}

	}
	
	
}
#if 0
int handle_delete(int cfd,struct send_pack recv_pack,struct current_data temp)
{
	extern sqlite3 *db;
	extern Link cb_head;
	Link p;

	search_data(db,temp.current_name,"dec","friend",INPORT);
	p = cb_head->next;
	while(p != NULL)
	{
		if(recv_pack.send_id1 == p->id1)
		{
			delete_by_id(db,temp.current_name,)
			break;
		}
		p = p->next;
	}
}
#endif
int press_anykey(int a,int b)
{
	if(b == 1)
	{
		printf("1 >> continue ");
		printf("2 >> stop\n");
		
	}
	if(b == 0)
	{
		printf("> Press any key to continue: ");
	}
	int buf;
	scanf("%d",&buf);
	fflush(stdin);
	if(a == 1)
	{
		system("clear");
	}
	
	return buf;
}

int handle_change(struct send_pack recv,int cfd,struct current_data *temp)
{
	extern sqlite3 *db;
	extern Link cb_head;
	char buf[MAX_SIZE] = {0};
	char buf1[MAX_SIZE] = {0};
	char buf2[MAX_SIZE] = {0};
	struct send_pack sdpk;
	int count = 1;

	memset(&sdpk,0,sizeof(sdpk));
	search_data(db,"userdata","name",(*temp).current_name,INPORT);
	switch(recv.send_id1)
	{
		case 1:
		{
			strcpy(buf,"name");
			memcpy(buf1,(cb_head->next)->name,strlen((cb_head->next)->name));
			count = search_data(db,"userdata","name",recv.send_msg0,COUNT);
			break;
		}

		case 2:
		{
			strcpy(buf,"pswd");
			memcpy(buf1,(cb_head->next)->pswd,strlen((cb_head->next)->pswd));
			break;			
		}

		case 3:
		{
			strcpy(buf,"ques");
			memcpy(buf1,(cb_head->next)->ques,strlen((cb_head->next)->ques));
			break;
		}

		case 4:
		{
			strcpy(buf,"answ");
			memcpy(buf1,(cb_head->next)->answ,strlen((cb_head->next)->answ));
			break;
		}
	}
	if(count == 0)
	{
		sprintf(buf2,"%d",(*temp).current_id1);
		update_data(db,"userdata",buf,recv.send_msg0,"id1",buf2,3);
		sdpk.send_cmd = INFO;
		sprintf(sdpk.send_msg0,"Your %s has been updated from %s to %s !",buf,buf1,recv.send_msg0);
		send_pk(&sdpk,cfd);
		if(strcmp(buf,"name") == 0)
		{	
			memset((*temp).current_name,0,sizeof((*temp).current_name));
			memcpy((*temp).current_name,recv.send_msg0,strlen(recv.send_msg0));
		}
	}
	else
	{
		sdpk.send_cmd = INFO;
		sprintf(sdpk.send_msg0,"Your new name %s has already been used !",recv.send_msg0);
		send_pk(&sdpk,cfd);
	}
}

int handle_signup(struct send_pack recv_pack,int cfd)
{
    extern sqlite3 *db;
	int flag = 0;
    int ret = 0;
    

    ret = search_data(db,"userdata","name",recv_pack.sourse_name,COUNT,NULL);
    
    if(ret == 0)
    {
        flag = search_data(db,"userdata",NULL,NULL,COUNT,NULL);
        recv_pack.send_id1 = flag + 1;
        insert_data(db,"userdata",recv_pack,3);
        printf("# Account %s signned , user id is %d\n",recv_pack.sourse_name,recv_pack.send_id1);
        recv_pack.result = SUCCESS;
        send_pk(&recv_pack,cfd);
    }
    
    else
    {
     	recv_pack.result = FAIL;
     	send_pk(&recv_pack,cfd);
    }
    
}

int handle_login(struct send_pack recv_pack,int cfd,struct current_data *temp)
{
    extern sqlite3 *db;
    extern Link online_user;
    extern Link cb_head;
    struct send_pack sdpk;
    int count = 0;
    int ret = 0;
    char buf[MAX_SIZE] = {0};
    
    ret = search_data(db,"userdata","name",recv_pack.sourse_name,INPORT);
    
	
    if((memcmp(recv_pack.send_msg0,(cb_head->next)->pswd,MAX_SIZE) == 0) && (ret == 1))
    {

        memcpy((*temp).current_name,recv_pack.sourse_name,strlen(recv_pack.sourse_name));
		(*temp).current_id1 = (cb_head->next)->id1;
		
		

		Link new = NULL;
		init_node(&new);
		new->fd = cfd;
		new->id1 = (*temp).current_id1;
		memset(new->name,0,MAX_SIZE);
		memcpy(new->name,(*temp).current_name,strlen((*temp).current_name));

		insert_node(new,online_user);

		printf("# %s log in , id is %d , cfd is %d\n",new->name,new->id1,new->fd);
		
		memset(&recv_pack,0,sizeof(recv_pack));

        recv_pack.result = SUCCESS;
		recv_pack.send_id1 = (*temp).current_id1;
		memcpy(recv_pack.sourse_name,(*temp).current_name,strlen((*temp).current_name));
		send_pk(&recv_pack,cfd);

		create_table(db,(*temp).current_name,6);		
     }
     else
     {
        recv_pack.result = FAIL;
        send_pk(&recv_pack,cfd);
     }
}


int handle_refind(struct send_pack recv_pack,int cfd,struct current_data temp)
{
	extern sqlite3 *db;
	extern Link cb_head;
	int ret = 0;
	struct send_pack refind_pack;

	memset(&refind_pack,0,sizeof(refind_pack));
	ret = search_data(db,"userdata","name",recv_pack.sourse_name,INPORT);
    
	if(ret == 0)
	{
		refind_pack.result = FAIL;
		send_pk(&refind_pack,cfd);	
	}
	else if(ret == 1)
	{
		
		
		memcpy(refind_pack.send_msg0,(cb_head->next)->ques,strlen((cb_head->next)->ques));
		refind_pack.result = SUCCESS;
		send_pk(&refind_pack,cfd);
		memset(&refind_pack,0,sizeof(refind_pack));
		recv_pk(&refind_pack,cfd);
		if(memcmp((cb_head->next)->answ,refind_pack.send_msg0,MAX_SIZE) == 0)
		{
			memset(&refind_pack,0,sizeof(refind_pack));
			refind_pack.result = SUCCESS;
			memcpy(refind_pack.send_msg0,(cb_head->next)->pswd,sizeof((cb_head->next)->pswd));
			send_pk(&refind_pack,cfd);
		}
		else
		{
			refind_pack.result = FAIL;
			send_pk(&refind_pack,cfd);
		}
	}

}

int handle_send(struct send_pack recv_pack,int cfd,struct current_data temp)
{
	extern sqlite3 *db;
	extern Link online_user;
	extern Link cb_head; 
	extern char temp_name[MAX_SIZE];
	Link p;
	Link q;
	int temp_id = 0;
	int count = 0;
	struct send_pack recv;
	struct send_pack temp_data;
	memset(&recv,0,sizeof(recv));
	memset(&temp_data,0,sizeof(temp_data));
	switch(recv_pack.send_id1)
	{
		case 1:
		{
			count = search_by_id(db,"userdata","id1",recv_pack.send_id2,EXCHANGE);
			strcpy(recv.send_msg0,"unchecked");
			recv.send_id1 = temp.current_id1;
			memcpy(recv.send_msg1,recv_pack.send_msg0,strlen(recv_pack.send_msg0));
			insert_data(db,temp_name,recv,6);
			p = online_user->next;
			while(p != NULL)
			{
				if(p->id1 == recv_pack.send_id2)
				{
					memset(&recv,0,sizeof(recv));
					recv.send_cmd = MUTI_SEND;
					memcpy(recv.send_msg0,recv_pack.send_msg0,strlen(recv_pack.send_msg0));
					memcpy(recv.sourse_name,temp.current_name,strlen(temp.current_name));
					send_pk(&recv,p->fd);
					break;
				}
				p = p->next;
			}
			break;
		}
		case 2:
		{
			count = search_data(db,"grouplist","name",recv_pack.sourse_name,COUNT);
			if(count != 0)
			{
				strcpy(temp_data.send_msg0,"message");
				temp_data.send_id1 = temp.current_id1;
				strcpy(temp_data.send_msg1,recv_pack.send_msg0);
				insert_data(db,recv_pack.sourse_name,temp_data,6);
				search_data(db,recv_pack.sourse_name,"dec","member",INPORT);
				p = cb_head->next;
				while(p != NULL)
				{
					memset(&temp_data,0,sizeof(temp_data));
					strcpy(temp_data.send_msg0,"gunchecked");
					temp_data.send_id1 = temp.current_id1;
					sprintf(temp_data.send_msg1,"%s (from group %s)",recv_pack.send_msg0,recv_pack.sourse_name);
					insert_data(db,p->name,temp_data,6);

					q = online_user->next;
					while(q != NULL)
					{
						if(q->id1 == p->id1)
						{
							recv.send_cmd = MUTI_SEND;
							memcpy(recv.sourse_name,temp.current_name,strlen(temp.current_name));
							sprintf(recv.send_msg0,"%s (from group %s)",recv_pack.send_msg0,recv_pack.sourse_name);
							send_pk(&recv,q->fd);		
						}				
						q = q->next;
					}
					
					p = p->next;
				}
				
				recv.send_cmd = INFO;
				sprintf(recv.send_msg0,"# Message %s has been sent to group %s !",recv_pack.send_msg0,recv_pack.sourse_name);
				send_pk(&recv,cfd);

			}
			else
			{
				recv.send_cmd = INFO;
				sprintf(recv.send_msg0,"# Group %s does not exist !",recv_pack.sourse_name);
				send_pk(&recv,cfd);
			}
			break;
		}
		case 3:
		{
			p = online_user->next;
			while(p != NULL)
			{
				memset(&temp_data,0,sizeof(temp_data));
				strcpy(temp_data.send_msg0,"unchecked");
				temp_data.send_id1 = p->id1;
				memcpy(temp_data.send_msg1,recv_pack.send_msg0,strlen(recv_pack.send_msg0));
				insert_data(db,q->name,temp_data,6);

				recv.send_cmd = MUTI_SEND;
				memcpy(recv.sourse_name,temp.current_name,strlen(temp.current_name));
				sprintf(recv.send_msg0,"%s",recv_pack.send_msg0);
				send_pk(&recv,p->fd);	
				p = p->next;				
			}
			break;
		}
		case 4:
		{
			p = online_user->next;
			while(p != NULL)
			{
				recv.send_cmd = MUTI_SEND;
				memcpy(recv.sourse_name,temp.current_name,strlen(temp.current_name));
				sprintf(recv.send_msg0,"%s",recv_pack.send_msg0);
				send_pk(&recv,p->fd);	
				p = p->next;				
			}
			search_data(db,"userdata",NULL,NULL,INPORT);
			q = cb_head->next;
			while(q != NULL)
			{
				memset(&temp_data,0,sizeof(temp_data));
				strcpy(temp_data.send_msg0,"unchecked");
				temp_data.send_id1 = temp.current_id1;
				memcpy(temp_data.send_msg1,recv_pack.send_msg0,strlen(recv_pack.send_msg0));
				insert_data(db,q->name,temp_data,6);
				q = q->next;
			}
			break;

		}
	}

}



int handle_accept_friend(struct send_pack recv_pk,int cfd,struct current_data temp)
{
	int fd;
	extern sqlite3 *db;
	extern Link cb_head;
	extern char temp_name[MAX_SIZE];
	
	struct send_pack sdpk;
	char buf[MAX_SIZE] = {0};
	
	memset(&sdpk,0,sizeof(sdpk));

	search_by_id(db,"friendreq","id2",temp.current_id1,INPORT);
	if((cb_head->next)->id1 == recv_pk.send_id1)
	{
		
		
		search_by_id(db,"userdata","id1",recv_pk.send_id1,EXCHANGE);
		
		sdpk.send_id1 = recv_pk.send_id1;
		strcpy(sdpk.send_msg0,"friend");
		strcpy(sdpk.send_msg1,temp_name);
		insert_data(db,temp.current_name,sdpk,6);

		memset(&sdpk,0,sizeof(sdpk));
		sdpk.send_id1 = temp.current_id1;
		strcpy(buf,temp_name);
		search_by_id(db,"userdata","id1",temp.current_id1,EXCHANGE);
		strcpy(sdpk.send_msg0,"friend");
		strcpy(sdpk.send_msg1,temp_name);
		insert_data(db,buf,sdpk,6);

	}
	else
	{
		printf("error\n");
	}
}

int handle_add_friend(struct send_pack recv_pk,int cfd,struct current_data temp)
{
	extern sqlite3 *db;
	extern Link online_user;
	extern Link cb_head;
	extern char temp_name[MAX_SIZE];
	struct send_pack sdpk;
	struct send_pack temp_data;
	int count = 0;
	int flag = 0;
	
	Link p;
	Link q;
	p = online_user->next;
	
	memset(&sdpk,0,sizeof(sdpk));
	search_by_id(db,"userdata","id1",recv_pk.send_id1,EXCHANGE);
	search_data(db,temp.current_name,"dec","friend",COUNT);
	q = cb_head->next;
	while(q != NULL)
	{
		if(q->id1 == recv_pk.send_id1)
		{
			flag++;
			break;
		}
		q = q->next;
	}

	if(flag == 0)
	{
		memset(&temp_data,0,sizeof(temp_data));
		strcpy(temp_data.send_msg0,"unchecked");
		temp_data.send_id1 = temp.current_id1;
		temp_data.send_id2 = recv_pk.send_id1;
		insert_data(db,"friendreq",temp_data,5);

		while(p != NULL)
		{
			if(p->id1 == recv_pk.send_id1)
			{
				sdpk.send_cmd = INFO;
				sprintf(sdpk.send_msg0,"%s wants to be your friend !",temp.current_name);
				send_pk(&sdpk,p->fd);
				break;
			}
			p = p->next;
		}
	}
	else
	{
		sdpk.send_cmd = INFO;
		sprintf(sdpk.send_msg0,"# %s has been your friend !",temp_name);
		send_pk(&sdpk,cfd);
	}
	
}

int handle_group(int cfd,struct current_data temp)
{
	struct send_pack recv;
	struct send_pack temp0;
	extern sqlite3 *db;
	extern Link cb_head;
	Link p;
	
	char buf[MAX_SIZE] = {0};
	int ret = 0;
	int count = 0;
	int count1 = 0;

	memset(&recv,0,sizeof(recv));
	memset(&temp0,0,sizeof(temp0));
	recv_pk(&recv,cfd);
	
	switch(recv.send_cmd)
	{
		case 1:
		{
			memcpy(buf,recv.sourse_name,sizeof(recv.sourse_name));
			count = search_data(db,"grouplist",NULL,NULL,COUNT);
			recv.send_id1 = count + 1;
			insert_data(db,"grouplist",recv,2);
			sprintf(recv.send_msg0,"# Group %s has successfully created ! ,id is %d",buf,count + 1);
			recv.send_cmd = INFO;
			send_pk(&recv,cfd);
			create_table(db,recv.sourse_name,6);
			strcpy(temp0.send_msg0,"member");
			strcpy(temp0.send_msg1,temp.current_name);
			temp0.send_id1 = temp.current_id1;
			insert_data(db,recv.sourse_name,temp0,6);
			memset(&recv,0,sizeof(recv));
			memcpy(recv.send_msg0,"group",6);
			recv.send_id1 = count + 1;
			memcpy(recv.send_msg1,buf,strlen(buf));
			insert_data(db,temp.current_name,recv,6);
			break;
		}
		
		case 2:
		{
			memset(&recv,0,sizeof(recv));
			count = search_data(db,"grouplist",NULL,NULL,INPORT);
			p = cb_head->next;
			sprintf(recv.send_msg0,"\n|%-10s|%-10s|\n","groupid","groupname");
			recv.send_cmd = INFO;
			send_pk(&recv,cfd);
			while(p != NULL)
			{
				memset(&recv,0,sizeof(recv));

				sprintf(recv.send_msg0,"|%-10d|%-10s|",p->id1,p->name);
				recv.send_cmd = INFO;
				send_pk(&recv,cfd);
				p = p->next;
			}
			break;
		}
		
		case 3:
		{
			count = search_data(db,"grouplist","name",recv.sourse_name,COUNT);
			if(count == 1)
			{
				strcpy(temp0.send_msg0,"member");
				temp0.send_id1 = temp.current_id1;
				memcpy(temp0.send_msg1,temp.current_name,strlen(temp.current_name));
				insert_data(db,recv.sourse_name,temp0,6);

				memset(&temp0,0,sizeof(temp0));
				search_data(db,"grouplist","name",recv.sourse_name,INPORT);
				
				strcpy(temp0.send_msg0,"group");
				temp0.send_id1 = (cb_head->next)->id1;
				memcpy(temp0.send_msg1,recv.sourse_name,strlen(recv.sourse_name));
				insert_data(db,temp.current_name,temp0,6);

				sprintf(buf,"# User %s has join into group %s !",temp.current_name,recv.sourse_name);
				memset(&recv,0,sizeof(recv));
				strcpy(recv.send_msg0,buf);
				recv.send_cmd = INFO;
				send_pk(&recv,cfd);
			}
			else
			{
				memset(&recv,0,sizeof(recv));
				strcpy(recv.send_msg0,"# Invaild group name !");
				recv.send_cmd = INFO;
				send_pk(&recv,cfd);
			}
			break;
		}
		
		case 4:
		{
			count = search_data(db,"userdata","id1",recv.send_id1,COUNT);
			count1 = search_data(db,"grouplist","name",recv.sourse_name,COUNT);
			if((count == 1) && (count1 == 1))
			{
				strcpy(buf,recv.sourse_name);
				memcpy(temp0.send_msg0,"member",7);
				temp0.send_id1 = recv.send_id1;
				memcpy(temp0.send_msg1,"null",5);
				insert_data(db,recv.sourse_name,temp0,6);
				memset(&recv,0,sizeof(recv));
				sprintf(recv.send_msg0,"User %d has join group %s",temp0.send_id1,buf);
				recv.send_cmd = INFO;
				send_pk(&recv,cfd);

			}
			else
			{
				memset(&recv,0,sizeof(recv));
				strcpy(recv.send_msg0,"# Invaild group name or user id !");
				recv.send_cmd = INFO;
				send_pk(&recv,cfd);
			}

			break;
		}
		
		case 5:
		{
			break;
		}
		
		case 6:
		{
			break;
		}
		
		case 7:
		{
			break;
		}

		case 8:
		{
			count = search_data(db,temp.current_name,"dec","group",INPORT);
			p = cb_head->next;
			sprintf(recv.send_msg0,"\n|%-10s|%-10s|","id","name");
			recv.send_cmd = INFO;
			send_pk(&recv,cfd);
			while(p != NULL)
			{
				memset(&recv,0,sizeof(recv));
				sprintf(recv.send_msg0,"|%-10d|%-10s|",p->id1,p->message);
				recv.send_cmd = INFO;
				send_pk(&recv,cfd);
				p = p->next;
			}
			break;
		}

		case 9:
		{
			count = search_data(db,recv.sourse_name,"dec","member",INPORT);
			p = cb_head->next;
			sprintf(recv.send_msg0,"\n|%-10s|\n","memberid");
			recv.send_cmd = INFO;
			send_pk(&recv,cfd);
			while(p != NULL)
			{
				memset(&recv,0,sizeof(recv));
				sprintf(recv.send_msg0,"|%-10d|",p->id1);
				recv.send_cmd = INFO;
				send_pk(&recv,cfd);
				p = p->next;
			}
			break;
		}
	}
}

int print_online_user()
{
	extern Link online_user;
	Link p;
	p = online_user->next;
	
	printf("|%-10s|%-10s|%-10s|\n","name","id","cfd");
	while(p != NULL)
	{
		printf("|%-10s|%-10d|%-10d|\n",p->name,p->id1,p->fd);
		p = p->next;
	}
	printf("\n");
}

int handle_message(int cfd,struct send_pack recv,struct current_data temp)
{
	extern sqlite3 *db;
	extern Link cb_head;
	extern char temp_name[MAX_SIZE];
	Link p;
	struct send_pack sdpk;
	memset(&sdpk,0,sizeof(sdpk));

	switch(recv.send_id1)
	{
		case 1:
		{
			search_data(db,temp.current_name,"dec","gunchecked",INPORT);
			p = cb_head->next;
			while(p != NULL)
			{
				search_by_id(db,"userdata","id1",p->id1,EXCHANGE);
				sdpk.send_cmd = MUTI_SEND;
				memcpy(sdpk.sourse_name,temp_name,strlen(temp_name));
				memcpy(sdpk.send_msg0,p->message,strlen(p->message));
				send_pk(&sdpk,cfd);
				update_data(db,temp.current_name,"dec","gchecked","message",p->message,1);
				p = p->next;
			}
			break;
		}
		case 2:
		{
			search_data(db,temp.current_name,"dec","unchecked",INPORT);
			p = cb_head->next;
			while(p != NULL)
			{
				search_by_id(db,"userdata","id1",p->id1,EXCHANGE);
				sdpk.send_cmd = MUTI_SEND;
				memcpy(sdpk.sourse_name,temp_name,strlen(temp_name));
				memcpy(sdpk.send_msg0,p->message,strlen(p->message));
				send_pk(&sdpk,cfd);
				update_data(db,temp.current_name,"dec","checked","message",p->message,1);
				p = p->next;
			}
			break;
		}
		case 3:
		{
			search_data(db,temp.current_name,"dec","gchecked",INPORT);
			p = cb_head->next;
			while(p != NULL)
			{
				search_by_id(db,"userdata","id1",p->id1,EXCHANGE);
				sdpk.send_cmd = MUTI_SEND;
				memcpy(sdpk.sourse_name,temp_name,strlen(temp_name));
				memcpy(sdpk.
					send_msg0,p->message,strlen(p->message));
				send_pk(&sdpk,cfd);
				p = p->next;
			}
			break;
		}
		case 4:
		{
			search_data(db,temp.current_name,"dec","checked",INPORT);
			p = cb_head->next;
			while(p != NULL)
			{
				search_by_id(db,"userdata","id1",p->id1,EXCHANGE);
				sdpk.send_cmd = MUTI_SEND;
				memcpy(sdpk.sourse_name,temp_name,strlen(temp_name));
				memcpy(sdpk.send_msg0,p->message,strlen(p->message));
				send_pk(&sdpk,cfd);
				p = p->next;
			}
			break;
		}
	}
}

int handle_file(int cfd,char *file_name,struct current_data temp)
{
	extern sqlite3 *db;
	struct send_pack pack;
	char buf[MAX_SIZE] = {0};
	char save_name[MAX_SIZE] = {0};// save at temp_file folder
 	
 	memset(&pack,0,sizeof(pack));
	sprintf(save_name,"./temp_file/%s",file_name);
    FILE *fp = fopen(save_name, "w");
    if(NULL == fp)
    {
        perror("open file");
        exit(1);
    }
 
    // 从服务器接收数据到buffer中
    // 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止
    
    int length = 0;
    while(1)
    {
    	length = recv(cfd,buf,sizeof(buf),0);
    	
        if(fwrite(buf,sizeof(char),length,fp) < length)
        {
            perror("write content");
            break;
        }
        if(length < MAX_SIZE)
        {
        	break;
        }
        memset(buf,0,MAX_SIZE);
        length = 0;
    }
    // 接收成功后，关闭文件，关闭socket
    close(fp);
    printf("# Receive File : %s\n", file_name);
    pack.send_id1 = temp.current_id1;
    memcpy(pack.sourse_name,file_name,strlen(file_name));
    insert_data(db,"filelist",pack,2);
}





int handle_fresh(int cfd,struct current_data temp)
{
	struct current_data temp_data;

	memset(&temp_data,0,sizeof(temp_data));
	memcpy(temp_data.current_name,temp.current_name,strlen(temp.current_name));
	temp_data.current_id1 = temp.current_id1;
	send(cfd,&temp_data,sizeof(temp_data),0);
}