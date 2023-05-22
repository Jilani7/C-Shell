#include <stdio.h> 
#include <errno.h> 
#include <netdb.h> 
#include <string.h>
#include <stdlib.h> 
#include <dirent.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LIMIT 30 


void checkHostName(int hostname) { 
    if (hostname == -1) { 
        perror("gethostname"); 
        exit(1); 
    } 
} 

void checkHostEntry(struct hostent * hostentry) { 
    if (hostentry == NULL) { 
        perror("gethostbyname"); 
        exit(1); 
    } 
}

int getRecursiveProcess(void) {
	int pidProcess = fork();
	if (pidProcess == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

void getUserName(void) {
	char *name = getenv ("USER");
	printf("<%s", name);
    struct hostent *host_entry;
	char hostbuffer[256]; 
	int hostname;
	hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
    checkHostName(hostname);
	host_entry = gethostbyname(hostbuffer); 
    checkHostEntry(host_entry); 
	printf(">@<%s", hostbuffer);
	char cwd[500];
   	if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("><%s", cwd);
	   printf("> > ");
   	} 
	else {
       perror("getcwd() error");
    }
}

char* inputFromUser(void) { 
	char buffer[1024];
    if (!fgets(buffer, sizeof buffer, stdin)) {
        return NULL;
    }
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[--len] = '\0';
    } 
	else {
    }
    char *str = (char*) malloc(len + 1);
    if (!str) {
        return NULL;
    }
    return strcpy(str, buffer);
} 

int main(int argc, char *argv[], char* envp[]) {
	while(1) {
	getUserName();
	char *userInput = inputFromUser();

	if (strcmp(userInput, "exit") == 0) {
		exit(0);
	}
	else if (strcmp(userInput, "clear") == 0){
		system("clear");
	}
	else if (strcmp(userInput, "pwd") == 0) {
	char cwd[500];
   	if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("%s", cwd);
	   }
	   else {
			perror("getcwd() error");
		}
	}
	else if (userInput[0] == 'l' && userInput [1] == 's' && (userInput[3]) != '|' && strlen(userInput) >= 2) {
		if (strlen(userInput) > 2) {
			char splitStrings[10][10];
			int i,j,cnt;
			j=0; cnt=0;
			for(i=0;i<=(strlen(userInput));i++)
			{
				if(userInput[i]==' '||userInput[i]=='\0')
				{
					splitStrings[cnt][j]='\0';
					cnt++;  
					j=0;   
				}
				else
				{
					splitStrings[cnt][j]=userInput[i];
					j++;
				}
			}
			DIR *direct;
			struct dirent *dir;
			direct = opendir(splitStrings[1]);
			if (direct){
				while ((dir = readdir(direct)) != NULL)
				{
					printf("%s\n", dir->d_name);
				}
				closedir(direct);
			}
			else {
				printf("\nNo such Directory Found\n");
			}
		}
		else {
			DIR *direct;
			struct dirent *dir;
			direct = opendir(".");
			if (direct) {
				while ((dir = readdir(direct)) != NULL)
				{
					printf("%s\n", dir->d_name);
				}
				closedir(direct);
			}
		}
	}
	else if (userInput[0] == 'c' && userInput[1] == 'd' && strlen(userInput) >=2) {
		if (strlen(userInput) > 2) {
			int j = 0; 
			int cnt = 0;
			char splitStrings[10][10];
			int i;
			for(i = 0; i <= (strlen(userInput)); i++) {
				if(userInput[i] == ' '|| userInput[i] == '\0')
				{
					splitStrings[cnt][j]='\0';
					++cnt;  
					j = 0;   
				}
				else
				{
					splitStrings[cnt][j]=userInput[i];
					j++;
				}
			}
			char changeDirectory[100];
			chdir(splitStrings[1]); 
    		printf("%s\n", getcwd(changeDirectory, 100));
		}
		else {
			char changeDirectory[100];
			chdir(".."); 
    		printf("%s\n", getcwd(changeDirectory, 100));
		}
	}
	else if (strcmp(userInput, "environ") == 0) {
		for (int i = 0; envp[i] != NULL; i++){    
			printf("\n%s", envp[i]);
		}
	}
	else if (userInput[0] == 's' && userInput[1] == 'e' && userInput [2] == 't' 
	&& userInput[3] == 'e' && userInput[4] == 'n'&& userInput[5] == 'v' && strlen(userInput) >= 7){

		char splitStrings[10][10];
		int i,j,cnt;
		j=0; cnt=0;
		for(i=0;i<=(strlen(userInput));i++)
		{
			if(userInput[i]==' '||userInput[i]=='\0')
			{
				splitStrings[cnt][j]='\0';
				cnt++;  
				j=0;   
			}
			else
			{
				splitStrings[cnt][j]=userInput[i];
				j++;
			}
		}
		char *path = splitStrings[1];
		char *input = splitStrings[2];
		putenv(strcat(path, input));
		char* pPath;
		pPath = getenv("PATH");
		printf("%s\n", pPath);
	}
	else if (strcmp(userInput, "ps") == 0) {
		pid_t pid = fork();
		if (pid < 0) {
			perror("fork");
		} 
		else if (pid == 0) {
			char *const a[] = {(char*)"ps",(char*)"-o",(char*)"pid,ppid,time",NULL};
			execvp("/bin/ps", a);
		}
		else if (pid >= 1) {
			wait(NULL);
		}
	}
	else if (strcmp(userInput, "top") == 0) {
		pid_t pid = fork();
		if (pid < 0) {
			perror("fork");
		} 
		else if (pid == 0) {
			char *const a[] = {(char*)"top",(char*)"-o",NULL};
			execvp("/bin/top", a);
		}
		else if (pid >= 1) {
			wait(NULL);
		}
	}
	// Condition to run background execuation...
	else if (strcmp(userInput, "dif") == 0) {
		int status;
		int pid = fork();
		if (pid == 0) {
			static char *args[] = {(char*)"./a", NULL};
			printf("\nBackground Execuation running\n\n");
            execv(args[0], args);
		} 
		else if (pid >= 1){
			waitpid(pid, &status, 0);
		}
	} 
	else if (userInput[3] == '|') {

		char splitStrings[10][10];
		int i,j,cnt;
		j=0; cnt=0;
		for(i=0;i<=(strlen(userInput));i++)
		{
			if(userInput[i]==' '||userInput[i]=='\0')
			{
				splitStrings[cnt][j]='\0';
				cnt++;  
				j=0;   
			}
			else
			{
				splitStrings[cnt][j]=userInput[i];
				j++;
			}
		}
		char *command = splitStrings[0];
		char *oper = splitStrings[2];
		char *sear = splitStrings[3];

		printf("COmmand = %s", command);
		printf("oper = %s", oper);
		printf("sear = %s", command);

		int fd[2];
		int status;
		pipe(fd);
		int pid;
		pid  = fork();
		if (pid >= 1)
		{	
			waitpid(pid, &status, 1);
			wait(NULL);
			close(fd[1]);   
			dup2(fd[0],0);
			execlp(oper,oper,sear,NULL);

		}
		else if (pid == 0)  {
			close(fd[0]);
			dup2(fd[1],1);
			execlp(command, command, NULL);
		}
		else {
			perror("Fork Error");
		}
	}
	else {
		char* token;
		printf("\nInvalid Command\n");
		char delim[] = " ";
		for (token = strtok(userInput, delim); token; token = strtok(NULL, delim))
		{
			printf("%s\n", token);
		}
	}
	printf("\n");	
	}
	exit(0);
}
