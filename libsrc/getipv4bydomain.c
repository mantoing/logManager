///
/// @brief  : HTTP Client test
/// @file   : httpcli.c
/// @date   : 2024. 05. 07. (화) 11:02:11 KST
/// @author : jay
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
        return -1;
    }

	// 하나만
	p = result;
	ipv4 = (struct sockaddr_in *)p->ai_addr;
	inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);

    freeaddrinfo(result);
	return 1;
}