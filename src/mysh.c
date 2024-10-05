#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fileA, *fileB;
    char *command, *args[10], termbuf[256];
    bool exit = false;

    while (!exit)
    {
        printf("$ ");
        fgets(termbuf, sizeof(termbuf), stdin);
        for (size_t i = 0; termbuf[i]; ++i)
        {
            if (termbuf[i] == '\n')
                termbuf[i] = '\0';
        }

        command = strtok(termbuf, " ");
        if (command == NULL)
            continue;
    
        if (strcmp(command, "cp") == 0)
        {
            args[0] = strtok(0, " ");
            if (args[0] == NULL)
            {
                fputs("Input file is missing\n", stderr);
                continue;
            }
            args[1] = strtok(0, " ");
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