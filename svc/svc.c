///
/// @brief  : TCP Server
/// @file   : svr.c
/// @date   : 2024. 05. 09. (목) 11:45:35 KST
/// @author : jaeywon
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

int main(int argc, char **argv)
{
	//소켓 fd 변환
	//loof 돌면서 10번 쓰고 받기
char sendbuf[1024];
char recvbuf[1024];
int ret;

	if (argc != 2){
		exit(1);
	}
	int sockfd = atoi(argv[1]);
	for (;;){
		memset(recvbuf, 0x00, 1024);
		ret = ReadStream2(sockfd, recvbuf);
		if(ret < 0){
			break;
		}
		sprintf(sendbuf, "[%d]%s", ret, recvbuf);
		ret = WriteStream2(sockfd, sendbuf, strlen(sendbuf));
		if(ret < 0){			
			break;
		}
	}
	exit(0);
}