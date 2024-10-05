#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fileA, *fileB;
    char *command, **args, termbuf[256];
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
    }

    return 0;
}