#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./multiple_wordcount file1 file2 ... filen\n");
        return 1;
    }

    int success_count = 0;
    int fail_count = 0;

    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {
            char *args[] = {"./wordcount", argv[i], NULL};
            if (execvp(args[0], args) == -1) {
                perror("exec failed");
                exit(2);
            }
        }
    }

    for (int i = 1; i < argc; i++) {
        int status;
        pid_t pid = wait(&status);
        if (pid != -1) {
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) == 0) {
                    success_count++;
                } else if (WEXITSTATUS(status) == 1) {
                    fail_count++;
                }
            }
        }
    }

    printf("Parent process %d created %d child processes to count words in %d files\n", getpid(), argc - 1, argc - 1);
    printf("%d files have been counted successfully!\n", success_count);
    printf("%d files did not exist\n", fail_count);

    return 0;
}