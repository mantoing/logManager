///
/// @brief  : TCP Server
/// @file   : svr.c
/// @date   : 2024. 05. 09. (목) 11:45:35 KST
/// @author : Cento
///
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include "jaeywon.h"

////////////////////////////////////////////////////////////////////////////////
/// SIGCHLD signal handler
/// @fn      void SigChild(int num)
/// @brief  signal handler
/// @param  num signal 번호
/// @return  없음
////////////////////////////////////////////////////////////////////////////////
void SigChild(int num)
{
int stat;
pid_t pid;
sigset_t block, oldset;
    sigfillset(&block);
    sigprocmask(SIG_BLOCK, &block, &oldset);
    while( (pid =  waitpid(-1, &stat, WNOHANG)) > 0);
    sigprocmask(SIG_SETMASK, &oldset, NULL);
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// Child Process 시작 Function (fork() & Execl() 사용)
/// @fn      ChildRun(int newfd)
/// @param   newfd : Child Process를 위한 socket fd 
/// @return  없음
////////////////////////////////////////////////////////////////////////////////
void ChildRun(int newfd)
{
pid_t pid;
char strfd[32];
	pid = fork();
	if(pid == -1){
		fprintf(stderr, "Error : fork() - %s\n", strerror(errno));
	}else if(pid == 0){
		// Child Process
		sprintf(strfd, "%d", newfd);
		printf("Child- newfd(%d) execl()을 수행합니다 \n", newfd);
		execl("./svc", "svc", strfd, NULL);
		// 여기 까지 오면 Error	
		fprintf(stderr, "Error execl() - %s\n", strerror(errno));
		exit(0);
	}else{
		// Parent process
		// 사용하지 않는 'newfd'는 닫기
		close(newfd);
	}
	return;
}
////////////////////////////////////////////////////////////////////////////////
/// main function
/// @fn     : int main(int argc,char **argv)
/// @param  :
/// @return :
////////////////////////////////////////////////////////////////////////////////
int main(int argc,char **argv)
{
int sockfd, newfd, len;
struct sockaddr_in cli_addr;

	if(argc != 2){
		fprintf(stderr, "TCP Server Process\n");
		fprintf(stderr, "Using) %s <port>\n", argv[0]);
		exit(0);
	}

	len = sizeof(cli_addr);
	sockfd = OpenInetStreamServer(atoi(argv[1]));
	if(sockfd < 0){
		fprintf(stderr, "Error! OpenInetStreamServer(%s)-%s\n",
			argv[1], strerror(errno));
		exit(0);
	}

	// 좀비를 막기위한 Signal 등록
	signal(SIGCHLD, SigChild);

	while(1){
		newfd = accept(sockfd, (struct sockaddr *)&cli_addr,(socklen_t *)&len);
		if(newfd < 0){
			fprintf(stderr, "Error. accept() - %s\n", strerror(errno));
			continue;
		}
		printf("accept() OK - newfd(%d)\n", newfd);
		ChildRun(newfd);
	}
}
