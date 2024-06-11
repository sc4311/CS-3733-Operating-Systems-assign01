#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to read a line from a file
char *readLine(FILE *file) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // Read a line from the file
    read = getline(&line, &len, file);

    // If read is -1, free the line and return NULL
    if (read == -1) {
        free(line);
        return NULL;
    }

    // Return the line read from the file
    return line;
}

int main(int argc, char *argv[]) {
    // Check if the number of arguments is correct
    if (argc != 2) {
        printf("Usage: ./wordcount filename\n");
        return 1;
    }

    // Open the file
    FILE *file = fopen(argv[1], "r");
    // If the file cannot be opened, print an error message and return 1
    if (file == NULL) {
        printf("wordcount with process %d cannot open %s\n", getpid(), argv[1]);
        return 1;
    }

    int count = 0;
    char *line;

    // Read lines from the file until there are no more
    while ((line = readLine(file)) != NULL) {
        // Tokenize the line into words
        char *word = strtok(line, " \t\n\r");
        // Count the words
        while (word != NULL) {
            count++;
            word = strtok(NULL, " \t\n\r");
        }
        // Free the line
        free(line);
    }

    // Print the word count
    printf("wordcount with process %d counted words in %s: number of words is %d\n", getpid(), argv[1], count);

    // Close the file
    fclose(file);
    return 0;
}