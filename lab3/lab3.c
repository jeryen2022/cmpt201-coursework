#define _POSIX_C_SOURCE 200890L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *user_input(void) {
  char *buff = NULL;
  size_t size = 0;
  printf("Enter input: ");
  if (getline(&buff, &size, stdin) == -1) {
    perror("getline failed");
    free(buff);
    exit(EXIT_FAILURE);
  }
  return buff;
}

int main(void) {
  while (1) {
    char *buff = NULL;
    char *lines[5] = {NULL};
    int index = 0;
    int count = 0;
    do {
      buff = user_input();
      free(lines[index]);
      lines[index] = buff;
      index = (index + 1) % 5;
      if (count < 5)
        count++;
    } while (strcmp(buff, "print\n") != 0);

    int start = (count < 5) ? 0 : index;
    for (int i = 0; i < count; i++)
      printf("%s", lines[(start + i) % 5]);

    for (int i = 0; i < 5; i++)
      free(lines[i]);
  }
}
