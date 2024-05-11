//
// Description : Make client socket(TCP)
// File Name   : streamcli.c
// Date        : 2013. 10. 23. (수) 09:33:59 KST
// By          : YJYOON
//
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFF_LEN    4096

int OpenInetStreamClient(host, port)
char    *host;
int      port;
{
struct sockaddr_in  serv_addr;
struct hostent     *hp;
struct servent     *sp;
int                 sockfd, buflen, on;
char                buff[256];

	memset(&serv_addr,0x00,sizeof(serv_addr));

	memset(buff, 0x00, 256);
	hp = gethostbyname(host);	
	if(!hp) return -1;
	serv_addr.sin_family        = AF_INET;
	serv_addr.sin_port          = htons(port);
	serv_addr.sin_addr.s_addr   = ((struct in_addr *)(hp->h_addr))->s_addr;
   	if((sockfd=socket(AF_INET,SOCK_STREAM,0)) <0) return -3; 

	if(connect(sockfd,(struct sockaddr *) 
						&serv_addr,sizeof(serv_addr)) <0){
		close(sockfd);
		return -3;
	}

	on = 1;
	buflen = MAX_BUFF_LEN;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&buflen, sizeof(buflen));
	setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (char *)&buflen, sizeof(buflen));
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on));
	return sockfd;
}
