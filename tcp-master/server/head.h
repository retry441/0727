#define SIGNUP 1011
#define LOGIN 1000
#define CLIENT_EXIT 1001
#define REFIND 1002
#define VIEW 1003
#define DELETE 1004
#define OFFLINE 1005
#define MUTI_SEND 1006
#define SHOW 1007
#define FRIEND_REQ 1008
#define FRIEND_ACC 1009
#define GROUP 1010
#define CHANGE 1012
#define TAN_FILE 1013
#define FILE_REQ 1014
#define EXCHANGE 1015
#define INFO 1016
#define CHECK 1017
#define FRESH 1018

#define PRINT 200
#define INPORT 201
#define COUNT 202
#define REMOVE 203
#define MSG 204
#define ID 205

#define SUCCESS 3001
#define FAIL 3002

#define MAX_SIZE 128
#define PORT_NUM 9878

struct send_pack
{ 
    int send_cmd;
    int result;
    char sourse_name[MAX_SIZE];
    char send_msg0[MAX_SIZE];
    char send_msg1[MAX_SIZE];
    char send_msg2[MAX_SIZE];
    char send_msg3[MAX_SIZE];
    char send_msg4[MAX_SIZE];
    int send_id1;
    int send_id2;
};

struct current_data
{
    char current_name[MAX_SIZE]; 
    char current_pswd[MAX_SIZE];
    char current_ques[MAX_SIZE];
    char current_answ[MAX_SIZE];
    int current_id1;
    int current_id2;
    int fd;
};

struct node
{
    int fd;//用户登陆时所在客户机的sockfd号
    
    char name[MAX_SIZE];
    char pswd[MAX_SIZE];
    char ques[MAX_SIZE];
    char answ[MAX_SIZE];
    char message[MAX_SIZE];
    char dec[MAX_SIZE];
    int id1;
    int id2;
    
    struct node *next;
};
typedef struct node Node;
typedef struct node *Link;
