#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//avoid global use
char input[512] = { 0 };

void printPrompt();
void readInput();
void parseInput(char*, char**);
void executeCommand(char**);

int main(void)
{
	char line[1024];
	char entryDir[512] = { 0 };

	if (getcwd(entryDir, sizeof(entryDir)) == NULL)
	{
		perror("Error while requesting current working directory:");
		//EXIT?		
	}

	//while (fgets(line, sizeof(line), stdin) != NULL)
	while(1)
	{
		printPrompt();

		if (fgets(line, sizeof(line), stdin) == NULL)
		{
			perror("Error while reading from stdin:");
			//EXIT?
		}

		char *args[64];
        	char **next = args;
        	char *temp = strtok(line, " \n");
        	while (temp != NULL)
        	{
        		*next++ = temp;
			temp = strtok(NULL, " \n");
        	}

        	*next = NULL;

		if (strcmp(args[0], "exit") == 0)
			exit(EXIT_SUCCESS);
		else if (strcmp(args[0], "cd") == 0)
		{
			if (args[1] == NULL)
			{
				if (chdir(entryDir) < 0)
					perror("Error while changing directory:");
			}		
			else
			{
				if (chdir(args[1]) < 0)
					perror("Error while changing directory:");
			}
		}
		else if (strcmp(args[0], "wait") == 0)
		{
			//code for wait
			printf("wait...\n");
		}
		else
		{
        		executeCommand(args);
		}
    	}

    return EXIT_SUCCESS;	
}

void printPrompt()
{
	char host[128] = { 0 };
	char cdir[512] = { 0 };
	if (gethostname(host, sizeof(host)) == -1)
	{
		perror("Error while requesting hostname:");
		//EXIT?	
	}

	if (getcwd(cdir, sizeof(cdir)) == NULL)
	{
		perror("Error while requesting current directory:");
		//EXIT?		
	}

	strcat(host, ":");
	strcat(host, cdir);
	strcat(host, "$ ");
	printf("%s", host);
}

void executeCommand(char **inArguments)
{
	pid_t child_pid = fork(); /* fork a new process */
	if (child_pid < 0)
		fprintf(stderr, "Fork Failed");
	else if (child_pid == 0)
	{
		signal(SIGINT, SIG_IGN);
		if (execvp(*inArguments, inArguments) < 0)
		{ /* execute cmd */
			perror("Too bad");
			exit(EXIT_FAILURE);
		}
	} 

}
