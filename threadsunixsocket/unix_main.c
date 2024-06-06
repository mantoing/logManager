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
#include <pthread.h>
#include "jaeywon.h"

#define SOCKET_PATH "/tmp/unix_socket"

queue msg_queue = {0, 1000, {{0}}};
int unix_socket;
pthread_t sub_thread;

////////////////////////////////////////////////////////////////////////////////
/// 시그널 핸들러 함수
/// @fn      void signal_handler(int signum)
/// @brief   메시지 큐에 새 데이터가 있을 때 호출되어 데이터를 처리
/// @param   signum  : 시그널 번호 [IN]
/// @return  없음
////////////////////////////////////////////////////////////////////////////////
void signal_handler(int signum) {
    static int localpos = 0;

    while (localpos != msg_queue.wpos) {
        printf("Processing data: %s\n", msg_queue.data[localpos]);
        localpos = (localpos + 1) % 1000;
    }
}
////////////////////////////////////////////////////////////////////////////////
/// 서브 스레드 함수
/// @fn      void *sub_thread_function(void *arg)
/// @brief   서브 스레드가 Unix 도메인 소켓을 통해 데이터를 수신하고 처리
/// @param   arg  : 스레드 인자 [IN]
/// @return  없음
////////////////////////////////////////////////////////////////////////////////
void *sub_thread_function(void *arg) {
    struct sockaddr_un addr;
    int localpos = 0;

    // Unix 도메인 소켓에 연결
    int sub_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sub_socket == -1) {
        fprintf(stderr, "Error: socket - %s\n", strerror(errno));
        pthread_exit(NULL);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sub_socket, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "Error: connect - %s\n", strerror(errno));
        close(sub_socket);
        pthread_exit(NULL);
    }

    // 시그널 핸들러 설정
    signal(SIGUSR1, signal_handler);

    // 루프를 돌며 시그널을 기다림
    while (1) {
        pause();
    }

    close(sub_socket);
    pthread_exit(NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// 메인 함수
/// @fn      int main(int argc, char **argv)
/// @brief   서버 소켓을 열고 클라이언트 연결을 수락하여 메시지를 큐에 저장
/// @param   argc  : 입력 인자 개수 [IN]
/// @param   argv  : 입력 인자 배열 [IN]
/// @return  0:정상 종료, 기타:에러
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    struct sockaddr_un addr;
    int server_fd, new_socket;
    char buffer[1024] = {0};

    // 포트 번호 확인
    if (argc != 2) {
        fprintf(stderr, "TCP Server Process\n");
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // Unix 도메인 소켓 생성
    unix_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (unix_socket == -1) {
        fprintf(stderr, "Error: socket - %s\n", strerror(errno));
        exit(1);
    }

    // 소켓 주소 설정 및 바인드
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(unix_socket, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        fprintf(stderr, "Error: bind - %s\n", strerror(errno));
        close(unix_socket);
        exit(1);
    }

    // 서브 스레드 생성 및 시작
    if (pthread_create(&sub_thread, NULL, sub_thread_function, NULL) != 0) {
        fprintf(stderr, "Error: pthread_create - %s\n", strerror(errno));
        close(unix_socket);
        exit(1);
    }

    // 서버 소켓 열기
    if ((server_fd = OpenInetStreamServer(atoi(argv[1]))) < 0) {
        fprintf(stderr, "Error: OpenInetStreamServer(%s) - %s\n", argv[1], strerror(errno));
        exit(1);
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // 메인 루프: 클라이언트 연결 수락 및 데이터 처리
    while (1) {
        // 클라이언트 연결 수락
        if ((new_socket = accept(server_fd, NULL, NULL)) < 0) {
            fprintf(stderr, "Error: accept - %s\n", strerror(errno));
            continue;
        }

        // 클라이언트로부터 데이터 읽기
        if (ReadStream2(new_socket, buffer) > 0) {
            // 데이터를 메시지 큐에 저장
            strcpy(msg_queue.data[msg_queue.wpos], buffer);
            msg_queue.wpos = (msg_queue.wpos + 1) % 1000;

            // 서브 스레드에 시그널 보내기
            kill(getpid(), SIGUSR1);

            printf("Message added to queue: %s\n", buffer);
        }

        // 클라이언트 소켓 닫기
        close(new_socket);
    }
    
    close(server_fd);
    return 0;
}