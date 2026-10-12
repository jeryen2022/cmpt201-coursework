#define _DEFAULT_SOURCE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 128

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(const char *type) {
  perror(type);
  exit(EXIT_FAILURE);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format, (unsigned int)*(unsigned char *)data);

  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

int main(void) {
  void *heap = sbrk(256);
  const uint64_t block_size = 128;

  struct header *my_block1 = heap;
  struct header *my_block2 = (struct header *)((char *)heap + block_size);

  my_block1->size = block_size;
  my_block1->next = NULL;
  my_block2->size = block_size;
  my_block2->next = my_block1;

  size_t data_size = block_size - sizeof(struct header);

  unsigned char *first_data = (unsigned char *)(my_block1 + 1);
  unsigned char *second_data = (unsigned char *)(my_block2 + 1);

  for (size_t i = 0; i < data_size; i++) {
    first_data[i] = 0;
    second_data[i] = 1;
  }

  print_out("first block:       %p\n", &my_block1, sizeof(my_block1));
  print_out("second block:      %p\n", &my_block2, sizeof(my_block2));

  print_out("first block size:  %lu\n", &my_block1->size, sizeof(my_block1->size));
  print_out("first block next:  %p\n", &my_block1->next, sizeof(my_block1->next));

  print_out("second block size: %lu\n", &my_block2->size, sizeof(my_block2->size));
  print_out("second block next: %p\n", &my_block2->next, sizeof(my_block2->next));

  for (size_t i = 0; i < data_size; i++) {
    print_out("%u\n", &first_data[i], sizeof(unsigned char));
  }

  for (size_t i = 0; i < data_size; i++) {
    print_out("%u\n", &second_data[i], sizeof(unsigned char));
  }

  int waldo = 42;
  print_out("Where is waldo? %p\n", &waldo, sizeof(&waldo));

  return 0;
}
