///
/// @brief Make stream client socket
/// @file openstcli.c
/// @date  ....
/// @작성자
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
/// @brief 
/// @fn int OpenStreamCli( char *host, int port)
/// @param host 
/// @param port 
/// @return client socket, Fail : -1, -2, -3
///

int OpenStreamCli(char *host, int port)
{
	int sock_fd;
    struct sockaddr_in server_addr;

    // 소켓 생성
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return (errno * -1);
    }

    // 서버 주소 구성
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); //바이너리 바이트를 한쪽으로 맞춰주는 역할 
    
    // IPv4 주소를 네트워크 바이트 순서로 변환
    if(inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        return (errno * -1);
    }

    // 서버에 연결
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        return (errno * -1);
    }

    return sock_fd;
}