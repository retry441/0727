#include <stdio.h>
#include <string.h>
#include <unistd.h>
# include<termios.h>
# include<assert.h>

int getch()
{
	int c=0;
	struct termios org_opts, new_opts;
	int res=0;

	res=tcgetattr(STDIN_FILENO, &org_opts);
	assert(res==0);
	memcpy(&new_opts, &org_opts, sizeof(new_opts));
	new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
	c=getchar();
	res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
	assert(res==0);
	return c;
}


int passwd_input(char *passwd)//返回密码长度
{
	int i = 0;
	int len = 0;
	for(i=0;;i++)
	{
		passwd[i]=getch();
		if(passwd[i]=='\n')
		{
			passwd[i]='\0';
			break;
		}
		if(passwd[i]==19)
		{
			printf("\b \b");
			i=i-2;
		}
		else
			printf("*");
		if(i<0)
			passwd[0]='\0';
	}
	len = strlen(passwd);
	return len;

}