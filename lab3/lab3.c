#define _POSIX_C_SOURCE 200890L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *user_input() {
  char *buff = NULL;
  size_t size = 0;
  printf("Enter input: ");
  if (getline(&buff, &size, stdin) == -1) {
    perror("getline failed");
    exit(EXIT_FAILURE);
  }
  return buff;
}

int main(void) {
  char *buff = NULL;
  char *saveptr;
  char *ret;
  char *lines[5] = {NULL};
  int i = 0;
  do {
    //    printf("test %d\n", i);
    buff = user_input();
    free(lines[i]);
    ret = strtok_r(buff, "\n", &saveptr);
    lines[i] = ret;
    //  i = (i + 1) % 5;
    (i++);
  } while (i < 5);
  //  } while (ret != "print\n");

  for (int i = 0; i < 5; i++)
    printf("%s\n", lines[i]);

  free(buff);
  for (int i = 0; i < 5; i++)
    free(lines[i]);
  free(saveptr);
  free(ret);
  return 0;
}
