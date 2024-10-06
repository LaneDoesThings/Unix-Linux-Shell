#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define promptLength 64
#define bufferSize 256
#define argsAmount 16
#define invalidOptions 1
#define sizeToLarge 2

char *strremove(char *str, const char *sub);

int echo(char *options, char *params);
int prompt(char *options, char *params, char *promptText);
int cat(char *options, char *params);
int copy(char *options, char *params);
int delete(char *options, char *params);
int makedir(char *options, char *params);
int rmdir(char *options, char *params);

int main(int argc, char *argv[]) {
  char *command, *args[argsAmount], inputbuf[bufferSize];
  bool exit = false;
  char promptText[promptLength] = "$";

  while (!exit) {
    // Display the prompt and wait for user input
    printf("%s ", promptText);
    fgets(inputbuf, sizeof(inputbuf), stdin);

    // Remove trailing newline
    inputbuf[strcspn(inputbuf, "\n")] = '\0';

    command = strtok(inputbuf, " ");

    char *arguments = strdup(strtok(NULL, ""));

    args[0] = strchr(arguments, '-');
    if (args[0] != NULL) {
      args[0] = strtok(args[0], "- ");
      args[1] = strtok(NULL, "");
    } else
      args[1] = strdup(arguments);

    if (strcmp(command, "echo") == 0) {
      if (echo(args[0], args[1]) != EXIT_SUCCESS) {
        fputs("Echo command exited with the above error(s)\n", stderr);
      }
    } else if (strcmp(command, "PS1") == 0) {
      if (prompt(args[0], args[1], promptText) != EXIT_SUCCESS) {
        fputs("PS1 command exited with the above error(s)\n", stderr);
      }
    } else if (strcmp(command, "cat") == 0) {
      //   cat(args[0], args[1]);
    } else if (strcmp(command, "cp") == 0) {
      //   copy(args[0], args[1]);
    } else if (strcmp(command, "rm") == 0) {
      //   delete (args[0], args[1]);
    } else if (strcmp(command, "mkdir") == 0) {
      //   makedir(args[0], args[1]);
    } else if (strcmp(command, "rmdir") == 0) {
      //   rmdir(args[0], args[1]);
    }
  }

  return EXIT_SUCCESS;
}

int echo(char *options, char *params) {

  bool carrageReturn = true;

  if (options != NULL) {
    for (size_t i = 0; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'n':
        carrageReturn = false;
        break;
      default:
        fprintf(stderr, "Unreconized option: %c\n", option);
        return invalidOptions;
      }
    }
  }
  if (carrageReturn) {
    strcat(params, "\r\n");
  }

  printf("%s", params);

  return EXIT_SUCCESS;
}

int prompt(char *options, char *params, char *promptText) {
  if (options != NULL) {
    for (size_t i = 0; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      default:
        fprintf(stderr, "Unreconized option: %c\n", option);
        return invalidOptions;
      }
    }
  }
  if (strlen(params) > promptLength) {
    fprintf(stderr,
            "Prompt may be no longer than %d characters inputed prompt is %d "
            "characters\n",
            promptLength, strlen(promptText));
    return sizeToLarge;
  }

  strcpy(promptText, params);
  return EXIT_SUCCESS;
}
// int main(int argc, char *argv[])
// {
//     FILE *fileA, *fileB;
//     char *command, *args[10], termbuf[256];
//     bool exit = false;
//     char *prompt = "$";

//     while (!exit)
//     {
//         printf("%s ", prompt);
//         fgets(termbuf, sizeof(termbuf), stdin);

//         // Remove trailing newline
//         termbuf[strcspn(termbuf, "\n")] = '\0';

//         command = strtok(termbuf, " ");
//         if (command == NULL)
//             continue;

//         if (strcmp(command, "echo") == 0)
//         {
//             args[0] = strtok(NULL, "\0");
//             char *options = strchr(args[0], '-');
//             if (options != NULL)
//             {
//                 args[1] = strtok(strdup(options), " ");
//                 args[2] = strdup(args[0]);
//                 char *spaceCheck = strcat(strdup(args[1]), " ");
//                 args[2] = strremove(args[2], spaceCheck);
//                 args[2] = strremove(args[2], args[1]);
//                 args[2] = strtok(args[2], "\"");
//                 printf("%s", args[2]);
//             }
//             else
//             {
//                 args[2] = strtok(strdup(args[0]), "\"");
//                 printf("%s\r\n", args[2]);
//             }
//         }

//         else if (strcmp(command, "PS1") == 0)
//         {
//             args[0] = strtok(NULL, "\0");
//             prompt = strdup(args[0]);
//             continue;
//         }

//         else if (strcmp(command, "cat") == 0)
//         {
//             args[1] = strtok(NULL, " ");
//             if (args[1] == NULL)
//             {
//                 fputs("No input file specified\n", stderr);
//                 continue;
//             }
//             fileA = fopen(args[1], "r");
//             if (fileA == NULL)
//             {
//                 perror(args[1]);
//                 continue;
//             }

//             int c;
//             while ((c = getc(fileA)) != EOF)
//             {
//                 putc(c, stdout);
//             }
//             putc('\n', stdout);
//             fclose(fileA);
//         }

//         else if (strcmp(command, "cp") == 0)
//         {
//             args[0] = strtok(NULL, " ");
//             if (args[0] == NULL)
//             {
//                 fputs("Input file is missing\n", stderr);
//                 continue;
//             }
//             args[1] = strtok(NULL, " ");
//             if (args[1] == NULL)
//             {
//                 fputs("Output file is missing\n", stderr);
//                 continue;
//             }

//             fileA = fopen(args[0], "r");
//             if (fileA == NULL)
//             {
//                 perror(args[0]);
//                 continue;
//             }

//             fileB = fopen(args[1], "w");
//             if (fileB == NULL)
//             {
//                 perror(args[1]);
//                 continue;
//             }
//             int c;
//             while ((c = getc(fileA)) != EOF)
//             {
//                 putc(c, fileB);
//             }

//             fclose(fileA);
//             fclose(fileB);
//         }

//         else if (strcmp(command, "rm") == 0)
//         {
//             args[1] = strtok(NULL, " ");
//             if (args[1] == NULL)
//             {
//                 fputs("No file specified\n", stderr);
//                 continue;
//             }

//             int test = remove(args[1]);
//             continue;
//         }

//         else if (strcmp(command, "mkdir") == 0)
//         {
//             args[1] = strtok(NULL, " ");
//             if (args[1] == NULL)
//             {
//                 fputs("No directory specified\n", stderr);
//                 continue;
//             }

//             mkdir(args[1], 0775);
//         }

//         else if (strcmp(command, "rmdir") == 0)
//         {
//             args[1] = strtok(NULL, " ");
//             if (args[1] == NULL)
//             {
//                 fputs("No directory specified\n", stderr);
//                 continue;
//             }

//             remove(args[1]);
//         }

//         else if (strcmp(command, "exit") == 0)
//         {
//             exit = true;
//         }
//     }

//     return EXIT_SUCCESS;
// }

// From
// https://stackoverflow.com/questions/47116974/remove-a-substring-from-a-string-in-c
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