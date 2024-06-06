#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "jaeywon.h"

#define SOCKET_PATH "/tmp/unix_socket"

////////////////////////////////////////////////////////////////////////////////
/// Thread function to handle each client
/// @fn      void *ThreadRun(void *arg)
/// @param   arg : void pointer to the socket file descriptor
/// @return  없음
////////////////////////////////////////////////////////////////////////////////
void *ThreadRun(void *arg)
{
    int newfd = *((int *)arg);

    char sendbuf[1024];
    char recvbuf[1024];
    int ret;

    for (;;)
    {
        memset(recvbuf, 0x00, 1024);
        ret = read(newfd, recvbuf, 1024);
        if (ret <= 0){
            break;
        }
        sprintf(sendbuf, "[%d]%s", ret, recvbuf);
        ret = write(newfd, sendbuf, strlen(sendbuf));
        if (ret < 0){
            break;
        }
    }
    close(newfd);
}

////////////////////////////////////////////////////////////////////////////////
/// main function
/// @fn     : int main(int argc,char **argv)
/// @param  :
/// @return :
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    int sockfd, *newfd;
    struct sockaddr_un addr;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0){
        fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    unlink(SOCKET_PATH);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        fprintf(stderr, "Error binding socket: %s\n", strerror(errno));
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) < 0){
        fprintf(stderr, "Error listening on socket: %s\n", strerror(errno));
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        socklen_t len = sizeof(addr);
        newfd = malloc(sizeof(int));
		if (newfd == NULL){
            fprintf(stderr, "Memory allocation error\n");
            continue;
        }

        *newfd = accept(sockfd, (struct sockaddr *)&addr, &len);
        if (*newfd < 0){
            fprintf(stderr, "Error accepting connection: %s\n", strerror(errno));
            free(newfd);
            continue;
        }
        printf("accept() OK - newfd(%d)\n", *newfd);

        pthread_t tid;
        if (pthread_create(&tid, NULL, ThreadRun, newfd) != 0){
            fprintf(stderr, "Error creating thread: %s\n", strerror(errno));
            free(newfd);
            continue;
        }
        pthread_detach(tid);
    }

    close(sockfd);
    unlink(SOCKET_PATH);
    return 0;
}