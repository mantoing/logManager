#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 80 // Google의 HTTP 서비스는 기본 포트 80을 사용한다고 한다.

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char *server_ip = "223.130.192.248"; // Google의 IP 주소

    // 소켓 생성
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 구성
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); //바이너리 바이트를 한쪽으로 맞춰주는 역할 
    
    // IPv4 주소를 네트워크 바이트 순서로 변환
    if(inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // 서버에 연결
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    // HTTP 1.1 인 이유는 현재 웹프로토콜에서 HTTP 1.1 버전이 주로 사용되기 때문에 사용한다.
    // connection이 된 후에 바로 닫아줘서 불필요한 연결을 해제하기 위해서 이다.
    char *request = "GET / HTTP/1.1\r\nHost: www.naver.com\r\nConnection: close\r\n\r\n";
    if (send(sock_fd, request, strlen(request), 0) < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }
    //바이너리 데이터를 주고받는다고 한다 따라서 fwrite함수를 사용해서 출력하였다.
    char buffer[1024];
    int valread;
    while ((valread = read(sock_fd, buffer, sizeof(buffer))) > 0) {
        //memset(buffer, 0x00, 1024);
        printf("%.*s", valread, buffer);
        //fwrite(buffer, 1, valread, stdout);
    }
    close(sock_fd);

    return 0;
}
