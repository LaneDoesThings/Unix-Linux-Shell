/* Lane Wright*/

#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>

#define promptLength 64
#define bufferSize 256
#define argsAmount 16

/* Not worth creating a whole function for one printf call so macro time*/
#define unreconizedOption(option)                                              \
  fprintf(stderr, "Unreconized option: %c\n", option)
#define unspecifiedError -1
#define invalidOptions 1
#define sizeToLarge 2
#define noParamsSpecified 3

int echo(char *options, char *params);
int prompt(char *options, char *params, char *promptText);
int cat(char *options, char *params);
int copy(char *options, char *params);
int delete(char *options, char *params);
int makedir(char *options, char *params);
int remdir(char *options, char *params);
int exec(char *arguments);

char *strdup(const char *s);
struct dirent *readdir(DIR *__dirp);
DIR *opendir(const char *__name);

int main(int argc, char *argv[]) {
  char *command, *args[argsAmount], inputbuf[bufferSize];
  bool exit = false;
  char promptText[promptLength] = "$";

  while (!exit) {
    /* Display the prompt and wait for user input*/
    printf("%s ", promptText);
    fgets(inputbuf, sizeof(inputbuf), stdin);

    /* Remove trailing newline*/
    inputbuf[strcspn(inputbuf, "\n")] = '\0';

    command = strtok(inputbuf, " ");

    char *postCommand = strtok(NULL, "");
    /* If no arguments are given give an empty string instead of NULL*/
    char *arguments = "\0";
    if (postCommand != NULL)
      arguments = strdup(postCommand);

    /*char *arguments = strdup((postCommand == NULL) ? "\0" : postCommand);*/

    /* Get the options and arguments given to a command*/
    args[0] = strchr(arguments, '-');
    if (args[0] != NULL) {
      args[0] = strtok(args[0], "- ");
      args[1] = strtok(NULL, "");
    } else
      args[1] = strdup(arguments);

    if (command == NULL) {
      continue;
    } else if (strcmp(command, "echo") == 0) {
      if (echo(args[0], args[1]) != EXIT_SUCCESS) {
        printf("Try 'echo -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "PS1") == 0) {
      if (prompt(args[0], args[1], promptText) != EXIT_SUCCESS) {
        printf("Try 'PS1 -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "cat") == 0) {
      if (cat(args[0], args[1]) != EXIT_SUCCESS) {
        printf("Try 'cat -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "cp") == 0) {
      if (copy(args[0], args[1]) != EXIT_SUCCESS) {
        printf("Try 'cp -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "rm") == 0) {
      if (delete (args[0], args[1]) != EXIT_SUCCESS) {
        printf("Try 'rm -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "mkdir") == 0) {
      if (makedir(args[0], args[1]) != EXIT_SUCCESS) {
        printf("Try 'mkdir -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "rmdir") == 0) {
      if (remdir(args[0], args[1]) != EXIT_SUCCESS) {
        printf("Try 'rmdir -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "exec") == 0) {
      if (exec(arguments) != EXIT_SUCCESS) {
        printf("Try 'exec -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "exit") == 0) {
      exit = true;
      continue;
    } else {
      fprintf(stderr, "Unknown command entered: %s\n", command);
    }
  }
  return EXIT_SUCCESS;
}

int echo(char *options, char *params) {

  bool carrageReturn = true;

  if (options != NULL) {
    size_t i = 0;
    for (i = 0; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'n':
        carrageReturn = false;
        break;
      case 'h':
        printf("Usage: echo [OPTION]... [STRING]\n");
        printf("Outputs the entered text to the standard output.\n\n");
        printf("-n\t do not output the trailing carriage return\n\n");
        printf("-h\t display this help and exit\n");
        return EXIT_SUCCESS;
      default:
        unreconizedOption(option);
        return invalidOptions;
      }
    }
  }
  if (carrageReturn) {
    strcat(params, "\r\n");
  }

  /* If no arguments were given print an empty string otherwise print what was
     given*/
  printf("%s", (params == NULL) ? "\0" : params);

  return EXIT_SUCCESS;
}

int prompt(char *options, char *params, char *promptText) {
  if (options != NULL) {
    size_t i = 0;
    for (i = 0; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'h':
        printf("Usage: PS1 [OPTION]... [STRING]\n");
        printf("Changes the user prompt\n");
        printf("Prompt may be no longer than %d characters\n\n", promptLength);
        printf("-h\t display this help and exit\n");
        return EXIT_SUCCESS;
      default:
        unreconizedOption(option);
        return invalidOptions;
      }
    }
  }
  if (params == NULL) {
    fputs("No prompt given\n", stderr);
    return noParamsSpecified;
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

int cat(char *options, char *params) {
  FILE *in;
  char *infile = strtok(params, " ");

  if (options != NULL) {
    size_t i = 0;
    for (i = 0; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'h':
        printf("Usage: cat [OPTION]... [FILE]\n");
        printf("Outputs the contents of a file to the standard output\n\n");
        printf("-h\t display this help and exit\n");
        return EXIT_SUCCESS;
      default:
        unreconizedOption(option);
        return invalidOptions;
      }
    }
  }
  if (infile == NULL) {
    fputs("No input file specified\n", stderr);
    return noParamsSpecified;
  }

  in = fopen(infile, "r");
  if (in == NULL) {
    fprintf(stderr, "cat: %s: %s\n", infile, strerror(errno));
    return unspecifiedError;
  }

  char c;
  while ((c = getc(in)) != EOF) {
    putc(c, stdout);
  }
  putc('\n', stdout);
  fclose(in);

  return EXIT_SUCCESS;
}

int copy(char *options, char *params) {
  FILE *in, *out;
  char *infile = strtok(params, " ");
  char *outfile = strtok(NULL, " ");
  if (options != NULL) {
    size_t i = 0;
    for (i = 0; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'h':
        printf("Usage: cp [OPTION]... SRC DEST\n");
        printf("Copies the contents of SRC to DEST\n\n");
        printf("-h\t display this help and exit\n");
        return EXIT_SUCCESS;
      default:
        unreconizedOption(option);
        return invalidOptions;
      }
    }
  }
  if (infile == NULL) {
    fputs("No input file specified\n", stderr);
    return noParamsSpecified;
  }
  if (outfile == NULL) {
    fputs("No output file specified\n", stderr);
    return noParamsSpecified;
  }
  in = fopen(infile, "r");
  if (in == NULL) {
    fprintf(stderr, "cp: %s: %s\n", infile, strerror(errno));
    return unspecifiedError;
  }
  out = fopen(outfile, "w");
  if (out == NULL) {
    fprintf(stderr, "cp: %s: %s\n", outfile, strerror(errno));
    return unspecifiedError;
  }

  char c;
  while ((c = getc(in)) != EOF) {
    putc(c, out);
  }

  fclose(in);
  fclose(out);

  return EXIT_SUCCESS;
}

int delete(char *options, char *params) {
  char *infile = strtok(params, " ");
  if (options != NULL) {
    size_t i = 0;
    for (i = 0; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'h':
        printf("Usage: rm [OPTION]... [FILE]\n");
        printf("Deletes a file\n\n");
        printf("-h\t display this help and exit\n");
        return EXIT_SUCCESS;
      default:
        unreconizedOption(option);
        return invalidOptions;
      }
    }
  }
  if (infile == NULL) {
    fputs("No file specified\n", stderr);
    return noParamsSpecified;
  }
  if (remove(infile) != EXIT_SUCCESS) {
    fprintf(stderr, "rm: %s: %s\n", infile, strerror(errno));
    return unspecifiedError;
  }
  return EXIT_SUCCESS;
}

int makedir(char *options, char *params) {
  char *infile = strtok(params, " ");
  if (options != NULL) {
    size_t i = 0;
    for (i = 0; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'h':
        printf("Usage: mkdir [OPTION]... [DIRECTORY]\n");
        printf("Create a directory\n\n");
        printf("-h\t display this help and exit\n");
        return EXIT_SUCCESS;
      default:
        unreconizedOption(option);
        return invalidOptions;
      }
    }
  }
  if (infile == NULL) {
    fputs("No directory specified\n", stderr);
    return noParamsSpecified;
  }

  if (mkdir(infile, 0775) != EXIT_SUCCESS) {
    fprintf(stderr, "mkdir: %s: %s\n", infile, strerror(errno));
    return unspecifiedError;
  }
}

/* Should refactor in the futute as is identical to remove besides some flavor
   text*/
int remdir(char *options, char *params) {
  char *infile = strtok(params, " ");
  if (options != NULL) {
    size_t i = 0;
    for (i = 0; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'h':
        printf("Usage: rmdir [OPTION]... [DIRECTORY]\n");
        printf("Deletes a directory\n\n");
        printf("-h\t display this help and exit\n");
        return EXIT_SUCCESS;
      default:
        unreconizedOption(option);
        return invalidOptions;
      }
    }
  }
  if (infile == NULL) {
    fputs("No directory specified\n", stderr);
    return noParamsSpecified;
  }
  if (remove(infile) != EXIT_SUCCESS) {
    fprintf(stderr, "rm: %s: %s\n", infile, strerror(errno));
    return unspecifiedError;
  }
  return EXIT_SUCCESS;
}

int exec(char *arguments) {
  char *params = strtok(arguments, " ");
  char *options = strtok(NULL, "");

  int status;
  switch (fork()) {
  case -1:
    return EXIT_FAILURE;
  case 0:
    char *path = getenv("PATH");
    
    char *arr[] = {params, options, NULL};
    if(params[0] != '/')
    {
    size_t i;
    char *paths[256];
    paths[0] = strtok(path, ":");
    for (i = 1; i < 256; i++) {
      paths[i] = strtok(NULL, ":");
    }

    for (i = 0; i < 256; i++) {
      if (paths[i] == NULL)
        break;

      char *temp = strdup(paths[i]);
      char *pathFull = malloc(256);
      strcpy(pathFull, temp);
      strcat(strcat(pathFull, "/"), params);

      if (execv(pathFull, arr) < 0) {
        free(pathFull);
        continue;
      }
    }
    }
    else
    {
      
      execv(params, arr);
    }
    exit(EXIT_SUCCESS);
  default:
    wait(&status);
    fprintf(stderr, "Exit status: %d\n", WEXITSTATUS(status));
  }

  return EXIT_SUCCESS;
}