#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./head.h"
#include <sqlite3.h>
int db_mannage()
{
    extern sqlite3 *db;
    extern Link cb_head;
	printf("\n<< DATA BASE MANNAGER >>\n\n");
    printf("1 >> Start serving clients\n");
    printf("2 >> Shut down this server\n");
    printf("3 >> View signned user infomation\n");
    printf("4 >> Delete signned user\n");
    printf("5 >> Update signned user data\n");
	printf("\n");
    
    int ui_cmd;
	int ret;
	Link head = NULL;

    printf("> Input cmd to use function: ");
	scanf("%d",&ui_cmd);
	
	
    switch(ui_cmd)
    {
        
		

		
		case 1:
		{
			
			return SUCCESS;
			break;
		}
		
		case 2:
		{
			exit(1);
			break;
		}
		

    }
}

int client_mannage()
{
    
    
	int ui_cmd;
	int ret;
	system("clear");
    printf("<< ONLINE USER MANNAGER >>\n\n");
    printf("1 >> View online user\n");
    printf("2 >> Kick off online user\n");
    
	printf("\n");
	printf("> Input cmd to use function:\n");
    
	
    scanf("%d",&ui_cmd);
    fflush(stdin);
    
	switch(ui_cmd)
    {
        case 1:
		{
			print_online_user();
			press_anykey(0,0);
			break;
		}
        	
		case 2:
		{
			break;
		}
				
		
    }
}
