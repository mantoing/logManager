///
/// @brief  :
/// @file   : jaeywon.h
/// @date   : 2024. 05. 13. (월) 10:33:28 KST
/// @author : jaeywon
///

#ifndef JAEYWON_H
# define JAEYWON_H


typedef struct _queue {
    int wpos;
    int dcnt;
    char data[1000][1024];
} queue;

extern queue msg_queue;

extern int GetIpv4(char *domain, char *ip);
extern int OpenInetStreamServer(int port);
extern int OpenInetStreamClient(char *host, int port);
extern int ReadStream2(int fd, char *buff);
extern int WriteStream2(int fd, char *buff, int len);


#endif