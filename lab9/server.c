#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                                          \
  do {                                                                                             \
    perror(msg);                                                                                   \
    exit(EXIT_FAILURE);                                                                            \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  // TODO: print the message received from client
  // TODO: increase total_message_count per message

  int cfd = client->cfd;
  int client_id = client->client_id;
  free(client);

  char buf[BUF_SIZE];
  ssize_t num_read;

  while (1) {
    num_read = read(cfd, buf, BUF_SIZE - 1);
    if (num_read > 0) {
      buf[num_read] = '\0';

      pthread_mutex_lock(&count_mutex);
      total_message_count++;
      int msg_num = total_message_count;
      pthread_mutex_unlock(&count_mutex);
      printf("Msg #%4d; Client ID %d: %s", msg_num, client_id, buf);
      fflush(stdout);
    } else if (num_read == 0) {
      printf("Client %d disconnected.\n", client_id);
      break;
    } else {
      perror("read");
      break;
    }
  }
  close(cfd);
  printf("Ending thread for client %d\n", client_id);
  return NULL;
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  for (;;) {
    // TODO: create a new thread when a new connection is encountered
    // TODO: call handle_client() when launching a new thread, and provide
    // client_info
    int cfd = accept(sfd, NULL, NULL);
    if (cfd == -1) {
      perror("accept");
      continue;
    }
    pthread_mutex_lock(&client_id_mutex);
    int new_client_id = client_id_counter++;
    pthread_mutex_unlock(&client_id_mutex);

    printf("New client created! ID %d on socket FD %d\n", new_client_id, cfd);
    struct client_info *client = malloc(sizeof(struct client_info));
    if (client == NULL) {
      perror("malloc");
      close(cfd);
      continue;
    }

    client->cfd = cfd;
    client->client_id = new_client_id;

    pthread_t thread;
    if (pthread_create(&thread, NULL, handle_client, client) != 0) {
      fprintf(stderr, "Error creating thread\n");
      free(client);
      close(cfd);
      continue;
    }
    pthread_detach(thread);
  }
  if (close(sfd) == -1) {
    handle_error("close");
  }
  return 0;
}
