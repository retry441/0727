#include <stdio.h>
#include "./head.h"
#include <sqlite3.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

sqlite3 *db = NULL;
Link online_user = NULL;
Link cb_head = NULL;
char temp_name[MAX_SIZE] = {0};
int sockfd = 0;
int flag = 0;

struct current_data temp_data0;
struct current_data temp_data1;


int main()
{
	
	int ret;
	
	init_node(&online_user);
	init_node(&cb_head);
	system("clear");
    sqlite3_init();
	
	while(1)
	{
	    ret = db_mannage();
	    if(ret == SUCCESS)
	    {
	    	system("clear");
	        break;
        }
	}
    
    server_connect();
    return 0;
}
