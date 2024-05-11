#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) 
	{
    	perror("fork failed");
        exit(EXIT_FAILURE);
    } 
	else if (pid == 0) 
	{
        printf("자식프로세스 실행\n");
        sleep(10);
        printf("자식프로세스 종료\n");
        exit(EXIT_SUCCESS);
    } 
	else 
	{
        printf("부모 프로세스 실행\n");
        printf("부모 프로세스 종료\n");
        exit(EXIT_SUCCESS);
    }

    return 0;
}