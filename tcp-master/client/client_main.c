#include <stdio.h>
#include "./head.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int sockfd;
Link friend_req = NULL;
int flag = 1;
struct current_data temp_data0;//data of already log in user
struct current_data temp_data1;

int user_offline()
{
	extern int sockfd;
	extern struct current_data temp_data0;
	struct send_pack sdpk;
	memset(&sdpk,0,sizeof(sdpk));
	sdpk.send_cmd = OFFLINE;
	sdpk.send_id1 = temp_data0.current_id1;
	send(sockfd,&sdpk,sizeof(sdpk),0);
}

int main()
{
    int ret0,ret1;
    tcp_link();
	init_node(&friend_req);
	system("clear");
    while(1)
    {
        ret0 = user_ui();
		if(ret0 == SUCCESS)
		{
	   		while(1)
	   	    {
	        	ret1 = chat_ui();
		    	if(ret1 == SUCCESS)
		    	{
					user_offline();
		        	break;
		    	}
	    	}
		}
    }
    return 0;
}
