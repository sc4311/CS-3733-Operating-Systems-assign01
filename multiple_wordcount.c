#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Check if the number of arguments is correct
    if (argc < 2) {
        printf("Usage: ./multiple_wordcount file1 file2 ... filen\n");
        return 1;
    }

    int success_count = 0;
    int fail_count = 0;

    // Loop over all the files provided as arguments
    for (int i = 1; i < argc; i++) {
        // Create a new process
        pid_t pid = fork();

        // If fork failed, print an error message and exit
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        // If this is the child process
        if (pid == 0) {
            // Prepare the arguments for the wordcount program
            char *args[] = {"./wordcount", argv[i], NULL};
            // Execute the wordcount program
            if (execvp(args[0], args) == -1) {
                // If exec failed, print an error message and exit
                perror("exec failed");
                exit(2);
            }
        }
    }

    // Wait for all child processes to finish
    for (int i = 1; i < argc; i++) {
        int status;
        pid_t pid = wait(&status);
        // If wait was successful
        if (pid != -1) {
            // If the child process exited normally
            if (WIFEXITED(status)) {
                // If the exit status was 0, increment the success count
                if (WEXITSTATUS(status) == 0) {
                    success_count++;
                }
                // If the exit status was 1, increment the fail count
                else if (WEXITSTATUS(status) == 1) {
                    fail_count++;
                }
            }
        }
    }

    // Print the number of child processes created and the number of files
    printf("Parent process %d created %d child processes to count words in %d files\n", getpid(), argc - 1, argc - 1);
    // Print the number of files that have been counted successfully
    printf("%d files have been counted successfully!\n", success_count);
    // Print the number of files that did not exist
    printf("%d files did not exist\n", fail_count);

    return 0;
}