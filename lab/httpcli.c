///
/// @brief  : HTTP Client test
/// @file   : httpcli.c
/// @date   : 2024. 05. 07. (화) 11:02:11 KST
/// @author : Cento
///
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

////////////////////////////////////////////////////////////////////////////////
/// domain 을 IPV4로 변환
/// @fn      int GetIpv4(char *domain, char *ip)
/// @param   domain  : domain name [IN] ex) www.google.co.kr
/// @param   ip      : ip buffer   [OUT]
/// @return  0:Error, 1:성공
////////////////////////////////////////////////////////////////////////////////
int GetIpv4(char *domain, char *ip)
{
struct addrinfo *result;
struct addrinfo hints;
struct sockaddr_in  *ipv4;
struct addrinfo *p;

    memset((char *)&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // Use IPv4 addresses

    int status = getaddrinfo(domain, NULL, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
    }

	// 하나만
	p = result;
	ipv4 = (struct sockaddr_in *)p->ai_addr;
	inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);

    freeaddrinfo(result);
	return 1;
}
int main(int argc, char **argv)
{
int                 socketfd;
int                 port, on = 1, rtn;
char                ip[32], sndbuf[1024], rcv;
struct hostent     *hp;
struct sockaddr_in svr_addr;

	if(argc != 3){
		fprintf(stderr, "\nhttp client test\n");
		fprintf(stderr, "Using) %s <ipv4 or domain> <port>\n", argv[0]);
		exit(0);
	}
	// socket 생성
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd < 0){
		fprintf(stderr, "Error : socket() - %s\n", strerror(errno));
		exit(0);
	}
	
	// Port
	port = atoi(argv[2]);

	// IP
	memset(ip, 0x00, 32);
	if(GetIpv4(argv[1], ip) != 1){
		exit(0);
	}
	printf("\n%s IPV4 is %s\n\n", argv[1], ip);

	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port   = htons(port);
	
	hp = gethostbyname(ip);
	svr_addr.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;

	// Connect to server
	if(connect(socketfd,(struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0){
		close(socketfd);
		fprintf(stderr, " Fail : connect()- %s\n", strerror(errno));
		exit(0);
	}

	setsockopt(socketfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on));

	// Write GET 
	memset(sndbuf, 0x00, 255);
	sprintf(sndbuf, 
		"GET / HTTP/1.0\r\n"
		"Host: %s\r\n"
		"Accept: */*\r\n"
		"Connection: close\r\n\r\n",
		argv[1]);
	rtn = write(socketfd , sndbuf, strlen(sndbuf));
	printf("Write----[%d]\n%s\n", rtn, sndbuf);

	for(;;){
		rtn = read(socketfd, &rcv, 1);
		if(rtn <= 0) break;
		
		printf("%c", rcv);
	}
	close(socketfd);
}
