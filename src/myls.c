#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *path = malloc(256);
  char *options = malloc(16);
  char ignoreChar = '.';

  if (argc > 3) {
    fprintf(stderr, "Too many options entered");
    return EXIT_FAILURE;
  }

  switch (argc) {
  case 3:
    strcpy(path, argv[1]);
    strcpy(options, argv[2]);
    break;
  case 2:
    if (argv[1][0] == '-') {
      strcpy(options, argv[1]);
      strcpy(path, ".");
    } else {
      strcpy(path, argv[1]);
    }
    break;
  default:
    strcpy(path, ".");
    break;
  }

  if (options != NULL) {
    size_t i = 0;
    for (i = 1; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'a':
        ignoreChar = ' ';
        break;
      default:
        fprintf(stderr, "Unrecognized option: %c", option);
        return EXIT_FAILURE;
      }
    }
  }

  printf("path: %s\noptions: %s\n", path, options);

  DIR *dir;
  struct dirent *dirEntry;

  if ((dir = opendir(path)) == NULL) {
    fprintf(stderr, "cannot open directory: %s\n", dir);
    return EXIT_FAILURE;
  }

  while ((dirEntry = readdir(dir)) != NULL) {
    if (dirEntry->d_name[0] == ignoreChar)
      continue;
    printf("%s ", dirEntry->d_name);
  }

  printf("\n");
  free(path);
  free(options);
  return EXIT_SUCCESS;
}