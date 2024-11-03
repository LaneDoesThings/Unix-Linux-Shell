/*Lane Wright*/

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char *path = malloc(256);
  char *options = malloc(16);
  char *args[16];
  char ignoreChar = '.';

  if (argc > 3) {
    fprintf(stderr, "Too many options entered");
    return EXIT_FAILURE;
  }
  size_t i;
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      strcpy(options, argv[i]);
    } else
      strcpy(path, argv[i]);
  }
  if (strcmp(options, "\0") == 0)
    options = NULL;
  if (strcmp(path, "\0") == 0)
    strcpy(path, ".");

  if (options != NULL) {
    size_t i;
    for (i = 1; i < strlen(options); i++) {
      char option = options[i];

      switch (option) {
      case 'h':
        printf("Usage: ls [OPTION]... [PATH]\n");
        printf("Outputs the contents of [PATH] to the standard output.\n\n");
        printf("-h\t display this help and exit\n");
        return EXIT_SUCCESS;
      case 'a':
        ignoreChar = ' ';
        break;
      default:
        fprintf(stderr, "Unrecognized option: %c\n", option);
        return EXIT_FAILURE;
      }
    }
  }

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