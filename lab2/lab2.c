#define _POSIX_C_SOURCE 200890L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char *buff = NULL;
  size_t size = 0;
  while (1) {
    printf("Enter Command: ");
    ssize_t num_char = getline(&buff, &size, stdin);
    if (num_char == -1) {
      perror("getline failed");
      exit(EXIT_FAILURE);
    }
    buff[strlen(buff) - 1] = '\0';

    pid_t pid = fork();
    if (pid == 0) {
      execl(buff, buff, (char *)NULL);

      perror("execl");
      exit(EXIT_FAILURE);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
      perror("wait");
      free(buff);
      exit(EXIT_FAILURE);
    }
  }
  free(buff);
}
