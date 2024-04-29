#include "kernel/types.h"
#include "user/user.h"

#define RD 0 //pipe的读端
#define WR 1 //pipe的写端

int main() {

    char massage = 'P';
    int fd_c2p[2], fd_p2c[2];
    pipe(fd_c2p);
    pipe(fd_p2c); 

    int pid = fork(), exit_status = 0;

    if (pid < 0) {
        fprintf(2, "fork() error!\n");
        close(fd_c2p[RD]);
        close(fd_c2p[WR]);
        close(fd_p2c[RD]);
        close(fd_p2c[WR]);
        exit(1);
    }else if (pid == 0) {  // 子进程
        close(fd_p2c[WR]);
        close(fd_c2p[RD]);

        if (read(fd_p2c[RD], &massage, sizeof(char)) != sizeof(char)) {
            fprintf(2, "child read() error!\n");
            exit_status = 1;
        }else {
            fprintf(1, "%d: received pong\n", getpid());
        }

        if (write(fd_c2p[WR], &massage, sizeof(char)) != sizeof(char)) {
            fprintf(2, "child write() error!\n");
            exit_status = 1;
        }

    }else { // 父进程
        close(fd_p2c[RD]);
        close(fd_c2p[WR]);

        if (write(fd_p2c[WR], &massage, sizeof(char)) != sizeof(char)) {
            fprintf(2, "parent write() error!\n");
            exit_status = 1;
        }

        if (read(fd_c2p[RD], &massage, sizeof(char)) != sizeof(char)) {
            fprintf(2, "parent read() error!\n");
            exit_status = 1;
        }else {
            fprintf(1, "%d: received pong\n", getpid());
        }
    } 

    exit(exit_status);
}
