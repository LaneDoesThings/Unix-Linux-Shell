#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define promptLength 64
#define bufferSize 256
#define argsAmount 16
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

    char *postCommand = strtok(NULL, "");
    char *arguments = strdup((postCommand == NULL) ? "\0" : postCommand);

    args[0] = strchr(arguments, '-');
    if (args[0] != NULL) {
      args[0] = strtok(args[0], "- ");
      args[1] = strtok(NULL, "");
    } else
      args[1] = strdup(arguments);

    if (strcmp(command, "echo") == 0) {
      if (echo(args[0], args[1]) != EXIT_SUCCESS) {
        fputs("echo command exited with the above error(s)\n", stderr);
      }
      continue;
    } else if (strcmp(command, "PS1") == 0) {
      if (prompt(args[0], args[1], promptText) != EXIT_SUCCESS) {
        fputs("PS1 command exited with the above error(s)\n", stderr);
      }
      continue;
    } else if (strcmp(command, "cat") == 0) {
      if (cat(args[0], args[1]) != EXIT_SUCCESS) {
        fputs("cat command exited with the above error(s)\n", stderr);
      }
      continue;
    } else if (strcmp(command, "cp") == 0) {
      if (copy(args[0], args[1]) != EXIT_SUCCESS) {
        fputs("cp command exited with the above error(s)\n", stderr);
      }
      continue;
    } else if (strcmp(command, "rm") == 0) {
      if (delete (args[0], args[1]) != EXIT_SUCCESS) {
        fputs("rm command exited with the above error(s)\n", stderr);
      }
      continue;
    } else if (strcmp(command, "mkdir") == 0) {
      if (makedir(args[0], args[1]) != EXIT_SUCCESS) {
        fputs("rm command exited with the above error(s)\n", stderr);
      }
      continue;
    } else if (strcmp(command, "rmdir") == 0) {
      if (delete (args[0], args[1]) != EXIT_SUCCESS) {
        fputs("rmdir command exited with the above error(s)\n", stderr);
      }
      continue;
    } else if (strcmp(command, "exit") == 0) {
      exit = true;
      continue;
    } else {
      fprintf(stderr, "Unknown command entered: %s\n", command);
      printf("Available commands "
             "are:\n\techo\n\tPS1\n\tcat\n\tcp\n\trm\n\tmkdir\n\trmdir\nUse -h "
             "to see more info about a command.\n");
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

  printf("%s", (params == NULL) ? "\0" : params);

  return EXIT_SUCCESS;
}

int prompt(char *options, char *params, char *promptText) {
  if (params == NULL) {
    fputs("No prompt given\n", stderr);
    return noParamsSpecified;
  }
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

int cat(char *options, char *params) {
  FILE *in;
  char *infile = strtok(params, " ");
  if (infile == NULL) {
    fputs("No input file specified\n", stderr);
    return noParamsSpecified;
  }
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
  if (infile == NULL) {
    fputs("No input file specified\n", stderr);
    return noParamsSpecified;
  }
  if (outfile == NULL) {
    fputs("No output file specified\n", stderr);
    return noParamsSpecified;
  }
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
  if (infile == NULL) {
    fputs("No file specified\n", stderr);
    return noParamsSpecified;
  }
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
  if (remove(infile) != EXIT_SUCCESS) {
    fprintf(stderr, "rm: %s: %s\n", infile, strerror(errno));
    return unspecifiedError;
  }
  return EXIT_SUCCESS;
}

int makedir(char *options, char *params) {
  char *infile = strtok(params, " ");
  if (infile == NULL) {
    fputs("No directory specified\n", stderr);
    return noParamsSpecified;
  }
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

  if (mkdir(infile, 0775) != EXIT_SUCCESS) {
    fprintf(stderr, "mkdir: %s: %s\n", infile, strerror(errno));
    return unspecifiedError;
  }
}