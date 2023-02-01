#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Too few arguments. Usage: redirout outfilename command [option1] ...\n");
    }

    char *outfile = argv[1];
    char *command = argv[2];

    int fd;
    fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int pfd[2];
    pipe(pfd);
    pid_t pid = fork();
    if (pid == 0) {
        dup2(pfd[1], 1);
        close(pfd[0]);
        execvp(command, argv+2);
        perror("wc");
        exit(1);
    } else {
        close(pfd[1]);
        char buf[2048];
        int count;
        while(1) {
            count = read(pfd[0], buf, sizeof buf);
            if (count == 0) {
                break;
            } else if (count == -1) {
                perror("Open");
                exit(1);
            }
            write(fd, buf, count);
        }
        close(pfd[0]);
        close(fd);
    }
}