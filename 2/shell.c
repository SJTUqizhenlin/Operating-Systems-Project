#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

// store the current command
char command[100];
// store the history command
char history_command[100]={0};
// store the command after split
char argv[50][50];
// whether running background
int isbackground=0;
// num of argv
int num_argv;
/*枚举类型，依次表示一般命令，带输出重定向命令，带输入重定向命令，带管道命令*/
enum specify{NORMAL, OUT_REDIRECT, IN_REDIRECT, PIPE};


int analysis_command(){
	char *s = command;
	int i=0,j=0,state=0;
	// split the space
	strcat(command," ");
    while(*s){
		switch(state){
           	case 0:
           		if(!isspace(*s))
		   			state=1;
	       		else
		   			s++;
	       		break;
	   		case 1:
	       		if(isspace(*s)){
		   			argv[i][j]='\0';
		   			i++;
		   			j=0;
		   			state=0;
	       		}
	       		else{
		   			argv[i][j]=*s;
		   			j++;
	       		}
	       		s++;
	       		break;
		}
    }
    num_argv = i;
    if(num_argv==0)
    	return 0;
    if(strcmp(argv[0],"!!")==0){
    	if(!history_command[0]){
    		printf("No commands in history.\n");
    		return 0;
    	}
    	strcpy(command,history_command);
    	return analysis_command();
    }
    int t=0;
    while(command[t]){
    	history_command[t] = command[t];
    	++t;
    }
    strcpy(history_command,command);
	if(strcmp(argv[0],"exit")==0)
		exit(0);
	if(strcmp(argv[num_argv-1],"&")==0){
		isbackground=1;
		argv[num_argv-1][0]='\0';
		num_argv--;
	}
	return 1;
}

int run_command(){
	enum specify type=NORMAL;
	int i,j;
	int f;
	char *argv_tmp[50];
	char *argv_tmp2[50];
	char *filename;
	for(i=0;i<num_argv;++i){
		argv_tmp[i]=argv[i];
	}
	argv_tmp[i] = NULL;

	for(i=0;i<num_argv;++i){
		// judge whether is <
		if(strcmp(argv_tmp[i],"<")==0){
			f++;
			filename = argv_tmp[i+1];
			argv_tmp[i]=NULL;
			type = IN_REDIRECT;
		}
		// judge whether is >
		else if(strcmp(argv_tmp[i],">")==0){
			f++;
			filename = argv_tmp[i+1];
			argv_tmp[i]=NULL;
			type = OUT_REDIRECT;
		}
		// judge whether is pipe
		else if(strcmp(argv_tmp[i],"|")==0){
			f++;
			type = PIPE;
			argv_tmp[i]=NULL;
			for(j=i+1;j<num_argv;++j){
				argv_tmp2[j-i-1]=argv[j];
			}
			argv_tmp2[j-i-1] = NULL;
		}
	}

	int pid = fork();
	int in,out;
	int pd[2];
	if(pid<0){
		// error
		perror("fork error!");
		exit(0);
	}
	else if(pid>0){
		// father process
		if(!isbackground)
			waitpid(pid,NULL,0);
		isbackground = 0;
	}
	else{
		// child process
		switch(type){
			case NORMAL:
				execvp(argv_tmp[0],argv_tmp);
				break;
			case IN_REDIRECT:
			// < type
				in = open(filename,O_RDONLY);
				dup2(in,STDIN_FILENO);
				execvp(argv_tmp[0],argv_tmp);
				close(in);
				break;

			case OUT_REDIRECT:
			// > type
				out = open(filename,O_WRONLY|O_CREAT, 0666);
				dup2(out,STDOUT_FILENO);
				execvp(argv_tmp[0],argv_tmp);
				close(out);
				break;
			case PIPE:
				// get the pipe
				pipe(pd);
				int pid2 = fork();
				if(pid2<0){
					perror("fork error!");
					exit(0);
				}
				else if(pid2>0){
					//father process
					close(pd[1]);
					dup2(pd[0],STDIN_FILENO);
					execvp(argv_tmp2[0],argv_tmp2);
				}
				else{
					// child process
					close(pd[0]);
					dup2(pd[1],STDOUT_FILENO);
					execvp(argv_tmp[0],argv_tmp);
				}
				break;
		}
	}
	return 1;
}

int main(){
	while(1){
		printf("osh>");
		fflush(stdout);
		// get the input commands string and store into the command
		gets(command);
		if(!analysis_command())
			continue;
		// run the commands
		run_command();
	}
	return 1;
}