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
	//서버로 connect
	//svc로 read, write
	char *host;
	int  port;
	int sockfd;
	int ret;
	char sendbuf[1024];
	char recvbuf[1024];

	if (argc != 3){
		exit(1);
	}
	host = argv[1];
	port = atoi(argv[2]);
	sockfd = OpenInetStreamClient(host, port);
	if (sockfd < 0) {
		exit(1);
	}
	for (int i=0; i < 10; i++){
		sprintf(sendbuf, "Send Index [%d]", i); 
		ret = WriteStream2(sockfd, sendbuf, strlen(sendbuf));
		if (ret < 0) {
			break;
		}
		memset(recvbuf, 0x00, 1024);
		ret = ReadStream2(sockfd, recvbuf);
		printf("[%d]%s\n", ret, recvbuf);
	}
	close(sockfd);
	exit(0);
}
