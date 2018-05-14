#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define _MAX BUFSIZ

void printCurrentWorkingDirectory();
void readCommand();
void matchCommand();
void setEntryDirectory();
void emptyBuf();
//void CTRLChandler();

char cwd[_MAX];
char buf[_MAX];
char entryDir[_MAX];
//int ctrlC = 0;

int main(void)
{	
	//TODO darum noch kümmern, dass nur noch bei wait ctrl+c akzeptiert wird
	//signal(SIGINT, CTRLChandler);
	
	//empty the buffer before first usage
	emptyBuf();
	//save the directory in which the shell was started
	setEntryDirectory();	

	while(1)
	{
		printCurrentWorkingDirectory();
		readCommand();
		matchCommand();
		emptyBuf();
	}	
	return 0;
}

void readCommand()
{
	int i=0;

	while (1) 
	{
		buf[i]=getchar();
		if(buf[i]=='\n')	
		{
			break;
		}
		i++;
	}
}

void matchCommand()
{
	if (buf[0]=='c' && buf[1]=='d')
	{
		if (buf[2] == '\n')
		{
			//change to entry directory
			if (chdir(entryDir) == -1)
			{
				printf("Error: directory not available.");	
			}
		}
		else if (buf[2] == ' ')
		{
			if (buf[3] == '.' && buf[4] == '.')
			{
				if (chdir("..") == -1)
				{
					printf("Error: directory not available.\n");
				}
			}
			else
			{
				int j = 3;
				char path[_MAX] = "";
				while(buf[j] != '\n')
				{
					path[j-3] = buf[j];
					j++;
				}
				path[j-2] = '\n';
				if (chdir(path) == -1)
				{
					printf("Error: directory not available.\n");
				}
			}
		}
	}
	else if (buf[0]=='w' && buf[1]=='a'&& buf[2]=='i'&& buf[3]=='t'&& buf[4]==' ')
	{

	}
	else if (buf[0]=='e' && buf[1]=='x'&& buf[2]=='i'&& buf[3]=='t' && buf[4]=='\n')
	{
		exit(0);
	}
	else
	{
		if(system(buf) < 0)
			printf("Error while processing request.");
	}
}

void printCurrentWorkingDirectory()
{
	
   	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{

	}
	else
	{
		strcpy(cwd, ".");
	}

	strcat(cwd, "/>");
	printf("%s", cwd); 
}

void setEntryDirectory()
{
   	if (getcwd(entryDir, sizeof(entryDir)) != NULL)
	{

	}
	else
	{
		strcpy(entryDir, "./");
	}	
}

void emptyBuf()
{
	int i;
	for (i = 0; i < sizeof(buf) - 1; i++)
	{
		buf[i] = '\0';
	} 
}

/*
void  CTRLChandler(int sig)
{
	if (ctrlC == 1)
	{
		printf("\n");
		printCurrentWorkingDirectory();
		
	}
	else if (ctrlC == 0)
	{
		exit(0);
	}		
}
*/
