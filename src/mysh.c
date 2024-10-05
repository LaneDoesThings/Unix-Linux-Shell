#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strremove(char *str, const char *sub);

int main(int argc, char *argv[])
{
    FILE *fileA, *fileB;
    char *command, *args[10], termbuf[256];
    bool exit = false;
    char *prompt = "$";

    while (!exit)
    {
        printf("%s ", prompt);
        fgets(termbuf, sizeof(termbuf), stdin);

        // Remove trailing newline
        termbuf[strcspn(termbuf, "\n")] = '\0';

        command = strtok(termbuf, " ");
        if (command == NULL)
            continue;

        if (strcmp(command, "echo") == 0)
        {
            args[0] = strtok(NULL, "\0");
            char *options = strchr(args[0], '-');
            if (options != NULL)
            {
                args[1] = strtok(strdup(options), " ");
                args[2] = strdup(args[0]);
                char* test = strcat(strdup(args[1]), " ");
                args[2] = strremove(args[2], test);
                args[2] = strremove(args[2], args[1]);
                args[2] = strtok(args[2], "\"");
                printf("%s", args[2]);
            }
            else
            {
                args[2] = strtok(strdup(args[0]), "\"");
                printf("%s\r\n", args[2]);
            }
        }

        else if (strcmp(command, "PS1") == 0)
        {
            args[0] = strtok(NULL, "\0");
            prompt = strdup(args[0]);
            continue;
        }

        else if (strcmp(command, "cp") == 0)
        {
            args[0] = strtok(NULL, " ");
            if (args[0] == NULL)
            {
                fputs("Input file is missing\n", stderr);
                continue;
            }
            args[1] = strtok(NULL, " ");
            if (args[1] == NULL)
            {
                fputs("Output file is missing\n", stderr);
                continue;
            }

            fileA = fopen(args[0], "r");
            if (fileA == NULL)
            {
                perror(args[0]);
                continue;
            }

            fileB = fopen(args[1], "w");
            if (fileB == NULL)
            {
                perror(args[1]);
                continue;
            }
            int c;
            while ((c = getc(fileA)) != EOF)
            {
                putc(c, fileB);
            }

            fclose(fileA);
            fclose(fileB);
        }
    }

    return EXIT_SUCCESS;
}


//From https://stackoverflow.com/questions/47116974/remove-a-substring-from-a-string-in-c
char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}