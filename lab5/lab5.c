#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  struct header *curr = free_list_ptr;
  while (curr != NULL) {
    if (curr->size >= size) {
      return curr->id;
    }
    curr = curr->next;
  }
  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  uint64_t smallest_size = UINT64_MAX;
  struct header *curr = free_list_ptr;
  while (curr != NULL) {
    if (curr->size >= size && curr->size < smallest_size) {
      smallest_size = curr->size;
      best_fit_id = curr->id;
    }
    curr = curr->next;
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  uint64_t largest_size = 0;
  struct header *curr = free_list_ptr;
  while (curr != NULL) {
    if (curr->size >= size && curr->size > largest_size) {
      largest_size = curr->size;
      worst_fit_id = curr->id;
    }
    curr = curr->next;
  }
  return worst_fit_id;
}

void free_all_blocks(struct header *block1, struct header *block2, struct header *block3,
                     struct header *block4, struct header *block5) {
  free(block1);
  free(block2);
  free(block3);
  free(block4);
  free(block5);
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  if (free_block1 == NULL || free_block2 == NULL || free_block3 == NULL || free_block4 == NULL ||
      free_block5 == NULL) {
    perror("malloc failed");
    free_all_blocks(free_block1, free_block2, free_block3, free_block4, free_block5);
    return 0;
  }

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("first_fit_id = %d\n", first_fit_id);
  printf("best_fit_id = %d\n", best_fit_id);
  printf("worst_fid_id = %d\n", worst_fit_id);

  free_all_blocks(free_block1, free_block2, free_block3, free_block4, free_block5);

  return 0;
}

// Part 2:

// struct header {
// uint64_t size;
// int id;
// };

struct header *coalesce(struct header *head, struct header *new_block) {
  struct header *curr = head;
  struct header *prev = NULL;

  while (curr != NULL && curr < new_block) {
    prev = curr;
    curr = curr->next;
  }
  new_block->next = new_block;
  if (prev != NULL) {
    prev->next = new_block;
  } else {
    head = new_block;
  }

  struct header *merge = new_block;
  if (prev != NULL && (prev + prev->size == new_block)) {
    prev->size += new_block->size;
    prev->next = new_block->next;
    merge->next = prev;
  }
  return head;
}
