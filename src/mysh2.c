/*Lane Wright*/

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>

#define PROMPTLENGTH 64
#define BUFFERSIZE 256
#define ARGSAMOUNT 16

/* Not worth creating a whole function for one printf call so macro time*/
#define unreconizedOption(option)                                              \
  fprintf(stderr, "Unreconized option: %c\n", option)

int echo(char *options, char **params);
int prompt(char *options, char **params, char *promptText);
int cat(char *options, char **params);
int copy(char *options, char **params);
int delete(char *options, char **params);
int makedir(char *options, char **params);
int remdir(char *options, char **params);
int exec(char *options, char **params);
int ls(char *options, char **params);

void arraddchar(char arr[ARGSAMOUNT], char toAdd);
void arraddstr(char *arr[ARGSAMOUNT], char *toAdd);
char *paramaterstr(char **params);

/*strdup isn't in the C90 Standard but a POSIX standard so it needs a prototype
 * defined*/
char *strdup(const char *s);

int main(int argc, char *argv[]) {
  char *command, inputbuf[BUFFERSIZE] = {0};
  bool exit = false;
  char promptText[PROMPTLENGTH] = "$";

  while (!exit) {
    /* Display the prompt and wait for user input*/
    printf("%s ", promptText);
    fgets(inputbuf, sizeof(inputbuf), stdin);

    /* Remove trailing newline*/
    inputbuf[strcspn(inputbuf, "\n")] = '\0';

    char *paramaters[BUFFERSIZE] = {0}, *args[ARGSAMOUNT] = {0},
         options[ARGSAMOUNT] = {0};

    /*Initial tokenization*/
    args[0] = strtok(inputbuf, " ");

    size_t i;
    for (i = 1; i < ARGSAMOUNT; i++) {
      args[i] = strtok(NULL, " ");
    }

    command = args[0];

    for (i = 1; i < ARGSAMOUNT; i++) {
      if (args[i] == NULL)
        break;
      if (args[i][0] == '-') {
        size_t j;
        for (j = 1; j < strlen(args[i]); j++) {
          char toAdd = args[i][j];
          arraddchar(options, toAdd);
        }
      } else
        arraddstr(paramaters, args[i]);
    }

    if (command == NULL) {
      continue;
    } else if (strcmp(command, "echo") == 0) {
      if (echo(options, paramaters) != EXIT_SUCCESS) {
        printf("Try 'echo -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "PS1") == 0) {
      if (prompt(options, paramaters, promptText) != EXIT_SUCCESS) {
        printf("Try 'PS1 -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "cat") == 0) {
      if (cat(options, paramaters) != EXIT_SUCCESS) {
        printf("Try 'cat -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "cp") == 0) {
      if (copy(options, paramaters) != EXIT_SUCCESS) {
        printf("Try 'cp -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "rm") == 0) {
      if (delete (options, paramaters) != EXIT_SUCCESS) {
        printf("Try 'rm -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "mkdir") == 0) {
      if (makedir(options, paramaters) != EXIT_SUCCESS) {
        printf("Try 'mkdir -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "rmdir") == 0) {
      if (remdir(options, paramaters) != EXIT_SUCCESS) {
        printf("Try 'rmdir -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "exec") == 0) {
      if (exec(options, paramaters) != EXIT_SUCCESS) {
        printf("Try 'exec -h' for more information\n");
      }
      continue;
    } else if (strcmp(command, "ls") == 0) {
      if (ls(options, paramaters) != EXIT_SUCCESS) {
        printf("Try 'ls -h' for more information\n");
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

int echo(char *options, char **params) {

  bool carrageReturn = true;
  char *toPrint = paramaterstr(params);

  if (options != NULL) {
    size_t i;
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
        return EXIT_FAILURE;
      }
    }
  }
  if (carrageReturn) {
    strcat(toPrint, "\r\n");
  }

  /* If no arguments were given print an empty string otherwise print what was
     given*/
  printf("%s", (toPrint == NULL) ? "\0" : toPrint);

  free(toPrint);
  return EXIT_SUCCESS;
}

int prompt(char *options, char **params, char *promptText) {
  char *newPrompt = paramaterstr(params);

  if (options != NULL) {
    size_t i = 0;
    for (i = 0; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'h':
        printf("Usage: PS1 [OPTION]... [STRING]\n");
        printf("Changes the user prompt\n");
        printf("Prompt may be no longer than %d characters\n\n", PROMPTLENGTH);
        printf("-h\t display this help and exit\n");
        return EXIT_SUCCESS;
      default:
        unreconizedOption(option);
        return EXIT_FAILURE;
      }
    }
  }

  if (strlen(newPrompt) > PROMPTLENGTH) {
    fprintf(stderr,
            "Prompt may be no longer than %d characters inputed prompt is %d "
            "characters\n",
            PROMPTLENGTH, strlen(newPrompt));
    return EXIT_FAILURE;
  }

  strcpy(promptText, newPrompt);
  free(newPrompt);
  return EXIT_SUCCESS;
}

int cat(char *options, char **params) {
  FILE *in;
  char *infile = paramaterstr(params);

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
        return EXIT_FAILURE;
      }
    }
  }

  if (infile == NULL) {
    fputs("No input file specified\n", stderr);
    return EXIT_FAILURE;
  }

  in = fopen(infile, "r");
  if (in == NULL) {
    fprintf(stderr, "cat: %s: %s\n", infile, strerror(errno));
    return EXIT_FAILURE;
  }

  char c;
  while ((c = getc(in)) != EOF) {
    putc(c, stdout);
  }
  putc('\n', stdout);
  fclose(in);

  return EXIT_SUCCESS;
}

int copy(char *options, char **params) {
  FILE *in, *out;
  char *infile = params[0];
  char *outfile = params[1];
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
        return EXIT_FAILURE;
      }
    }
  }
  if (infile == NULL) {
    fputs("No input file specified\n", stderr);
    return EXIT_FAILURE;
  }
  if (outfile == NULL) {
    fputs("No output file specified\n", stderr);
    return EXIT_FAILURE;
  }
  in = fopen(infile, "r");
  if (in == NULL) {
    fprintf(stderr, "cp: %s: %s\n", infile, strerror(errno));
    return EXIT_FAILURE;
  }
  out = fopen(outfile, "w");
  if (out == NULL) {
    fprintf(stderr, "cp: %s: %s\n", outfile, strerror(errno));
    return EXIT_FAILURE;
  }

  char c;
  while ((c = getc(in)) != EOF) {
    putc(c, out);
  }

  fclose(in);
  fclose(out);

  return EXIT_SUCCESS;
}

int delete(char *options, char **params) {
  char *infile = paramaterstr(params);
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
        return EXIT_FAILURE;
      }
    }
  }
  if (infile == NULL) {
    fputs("No file specified\n", stderr);
    return EXIT_FAILURE;
  }
  if (remove(infile) != EXIT_SUCCESS) {
    fprintf(stderr, "rm: %s: %s\n", infile, strerror(errno));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int makedir(char *options, char **params) {
  char *infile = paramaterstr(params);
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
        return EXIT_FAILURE;
      }
    }
  }
  if (infile == NULL) {
    fputs("No directory specified\n", stderr);
    return EXIT_FAILURE;
  }

  if (mkdir(infile, 0775) != EXIT_SUCCESS) {
    fprintf(stderr, "mkdir: %s: %s\n", infile, strerror(errno));
    return EXIT_FAILURE;
  }
}

/* Should refactor in the futute as is identical to remove besides some flavor
   text*/
int remdir(char *options, char **params) {
  char *infile = paramaterstr(params);
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
        return EXIT_FAILURE;
      }
    }
  }
  if (infile == NULL) {
    fputs("No directory specified\n", stderr);
    return EXIT_FAILURE;
  }
  if (remove(infile) != EXIT_SUCCESS) {
    fprintf(stderr, "rm: %s: %s\n", infile, strerror(errno));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int exec(char *options, char **params) {

  int status; /*return status*/
  switch (fork()) {
  case -1:
    return EXIT_FAILURE;
  case 0:
    char *path = getenv("PATH");

    char *toExec = params[0];
    char *toExecParams = malloc(BUFFERSIZE);
    strcpy(toExecParams, "\0");

    if (params[1] == NULL)
      toExecParams = NULL;
    else {
      size_t i;
      for (i = 1; i < ARGSAMOUNT; i++) {
        if (params[i] == NULL)
          break;
        if (i == 1)
          strcat(toExecParams, params[i]);
        else
          strcat(strcat(toExecParams, " "), params[i]);
      }
    }

    char *opts = malloc(sizeof(options) + sizeof(char) + 1);
    if (strcmp(options, "\0") != 0)
      strcat(strcat(strcpy(opts, "-"), options), "\0");
    else
      opts = NULL;

    char **arr;
    if (opts == NULL) {
      char *temp[] = {toExec, toExecParams, opts, NULL};
      arr = temp;
    } else {
      char *temp[] = {toExec, opts, toExecParams, NULL};
      arr = temp;
    }

    if (toExec[0] != '/') {
      size_t i;
      char *paths[BUFFERSIZE];
      paths[0] = strtok(path, ":");
      for (i = 1; i < BUFFERSIZE; i++) {
        paths[i] = strtok(NULL, ":");
      }

      for (i = 0; i < BUFFERSIZE; i++) {
        if (paths[i] == NULL)
          break;

        char *temp = strdup(paths[i]);
        char *pathFull = malloc(BUFFERSIZE);
        strcpy(pathFull, temp);
        strcat(strcat(pathFull, "/"), toExec);

        if (execv(pathFull, arr) < 0) {
          free(pathFull);
          continue;
        }
      }

    }
    /*absolute path was given so don't look through PATH*/
    else {
      execv(toExec, arr);
    }
    free(opts);
    free(toExecParams);
    exit(EXIT_SUCCESS);
  default:
    wait(&status);
    fprintf(stderr, "Exit status: %d\n", WEXITSTATUS(status));
  }

  return EXIT_SUCCESS;
}

/*Runs the myls program
  expects it to be compiled with no extension and
  in the same directorry as this file*/
int ls(char *options, char **params) {

  int status;
  switch (fork()) {
  case -1:
    return EXIT_FAILURE;
  case 0:

    char *toExec = "./myls";
    char *toExecParams = malloc(BUFFERSIZE);
    strcpy(toExecParams, "\0");

    if (params[0] == NULL)
      toExecParams = NULL;
    else {
      size_t i;
      for (i = 0; i < ARGSAMOUNT; i++) {
        if (params[i] == NULL)
          break;
        if (i == 0)
          strcat(toExecParams, params[i]);
        else
          strcat(strcat(toExecParams, " "), params[i]);
      }
    }

    char *opts = malloc(sizeof(options) + sizeof(char) + 1);
    if (strcmp(options, "\0") != 0)
      strcat(strcat(strcpy(opts, "-"), options), "\0");
    else
      opts = NULL;

    char **arr;
    if (opts == NULL) {
      char *temp[] = {toExec, toExecParams, opts, NULL};
      arr = temp;
    } else {
      char *temp[] = {toExec, opts, toExecParams, NULL};
      arr = temp;
    }
    execv(toExec, arr);
    free(opts);
    free(toExecParams);
    exit(EXIT_SUCCESS);
  default:
    wait(&status);
    fprintf(stderr, "Exit status: %d\n", WEXITSTATUS(status));
  }

  return EXIT_SUCCESS;
}

void arraddchar(char arr[ARGSAMOUNT], char toAdd) {
  size_t i;
  for (i = 0; i < ARGSAMOUNT; i++) {
    if (arr[i] == '\0') {
      arr[i] = toAdd;
      return;
    }
  }
}

void arraddstr(char *arr[ARGSAMOUNT], char *toAdd) {
  size_t i;
  for (i = 0; i < ARGSAMOUNT; i++) {
    if (arr[i] == NULL) {
      arr[i] = toAdd;
      return;
    }
  }
}

char *paramaterstr(char **params) {
  char *str = malloc(BUFFERSIZE);
  strcpy(str, "\0");

  size_t i;
  for (i = 0; i < ARGSAMOUNT; i++) {
    if (params[i] == NULL)
      break;
    if (i == 0)
      strcat(str, params[i]);
    else
      strcat(strcat(str, " "), params[i]);
  }

  return str;
}