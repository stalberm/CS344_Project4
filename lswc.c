#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if (argc > 2) {
        printf("Too many arguments. Usage: lswc [path]\n");
        exit(1);
    }

    char *directory = argv[1];
    int pfd[2];
    pipe(pfd);
    pid_t pid = fork();
    if (pid == 0) {
        dup2(pfd[0], 0);
        close(pfd[1]);
        execlp("wc", "wc", "-l", NULL);
        perror("wc");
        exit(1);
    } else {
        dup2(pfd[1], 1);
        close(pfd[0]);
        execlp("ls", "ls", "-1a", directory, NULL);
        perror("ls");
        exit(1);
    }
    return 0;
}
