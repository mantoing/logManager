///
/// @brief  :
/// @file   : jaeywon.h
/// @date   : 2024. 05. 13. (월) 10:33:28 KST
/// @author : jaeywon
///

#ifndef JAEYWON_H
# define JAEYWON_H

extern int GetIpv4(char *domain, char *ip);
extern int OpenInetStreamServer(int port);
extern int OpenInetStreamClient(char *host, int port);
extern int ReadStream2(int fd, char *buff);
extern int WriteStream2(int fd, char *buff, int len);


#endif