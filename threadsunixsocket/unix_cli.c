#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "jaeywon.h"

////////////////////////////////////////////////////////////////////////////////
/// 메인 함수
/// @fn      int main(int argc, char **argv)
/// @brief   서버에 연결하여 메시지를 전송
/// @return  0:정상 종료, 기타:에러
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
    int sockfd;
	int port;
    char *host;
	char sendbuf[1024];

	if (argc != 3){
		exit(1);
	}
	host = argv[1];
	port = atoi(argv[2]);
    sockfd = OpenInetStreamClient(host, port);
    if (sockfd < 0) {
        printf("Connection Failed\n");
        exit(1);
    }
    WriteStream2(sockfd, sendbuf, strlen(sendbuf));
    printf("Message sent\n");

    close(sockfd);

    return 0;
}






