#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

char *readLine(FILE *file) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, file);

    if (read == -1) {
        free(line);
        return NULL;
    }

    return line;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./wordcount filename\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("wordcount with process %d cannot open %s\n", getpid(), argv[1]);
        return 1;
    }

    int count = 0;
    char *line;

    while ((line = readLine(file)) != NULL) {
        char *word = strtok(line, " \t\n\r");
        while (word != NULL) {
            count++;
            word = strtok(NULL, " \t\n\r");
        }
        free(line);
    }

    printf("wordcount with process %d counted words in %s: number of words is %d\n", getpid(), argv[1], count);

    fclose(file);
    return 0;
}