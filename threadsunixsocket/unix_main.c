#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>
#include <errno.h>

#include "jaeywon.h"

#define PORT 8080
#define SOCKET_PATH "/tmp/unix_socket"

typedef struct _queue {
    int wpos;
    int dcnt;
    char data[1000][1024];
} queue;

queue msg_queue = {0, 1000, {{0}}};


////////////////////////////////////////////////////////////////////////////////
/// 신호 핸들러 함수
/// @fn      void signal_handler(int signum)
/// @brief   서브 프로세스에 데이터가 있다는 신호를 보냄
/// @param   signum  : 신호 번호 [IN]
/// @return  없음
////////////////////////////////////////////////////////////////////////////////
void signal_handler(int signum) {
    int unix_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (unix_socket == -1) {
        fprintf(stderr, "Error : socket - %s\n", strerror(errno));
        exit(1);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (sendto(unix_socket, "data", sizeof("data"), 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "Error : sendto() - %s\n", strerror(errno));
    }

    close(unix_socket);
}

////////////////////////////////////////////////////////////////////////////////
/// 메인 함수
/// @fn      int main(int argc, char **argv)
/// @brief   서버 소켓을 열고 클라이언트 연결을 수락하여 메시지를 큐에 저장
/// @return  0:정상 종료, 기타:에러
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    int server_fd, new_socket;
    char buffer[1024] = {0};

    if (argc != 2) {
		fprintf(stderr, "TCP Server Process\n");
		fprintf(stderr, "Using) %s <port>\n", argv[0]);
		exit(1);
    }

    if ((server_fd = OpenInetStreamServer(atoi(argv[1]))) < 0) {
        fprintf(stderr, "Error! OpenInetStreamServer(%s)-%s\n",
			argv[1], strerror(errno));
        exit(1);
    }

    while (1) {
        if ((new_socket = accept(server_fd, NULL, NULL)) < 0) {
            fprintf(stderr, "Error : accept - %s\n", strerror(errno));
            continue;
        }

        if (ReadStream2(new_socket, buffer) > 0) {
            strcpy(msg_queue.data[msg_queue.wpos], buffer);
            msg_queue.wpos = (msg_queue.wpos + 1) % 1000;

            signal_handler(SIGUSR1);

            printf("Message added to queue: %s\n", buffer);
        }

        close(new_socket);
    }

    close(server_fd);
    return 0;
}