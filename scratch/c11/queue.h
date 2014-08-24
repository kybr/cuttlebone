#ifndef CB_QUEUE_C
#define CB_QUEUE_C

#include "stdatomic.h"
#include <string.h>  // memcpy
#include <stdlib.h>  // for size_t

typedef struct q {
  size_t capacity, data_size;
  atomic_size_t head, tail;
  void** data;
} queue_t;

size_t queue_increment(queue_t* q, size_t i) { return (i + 1) % q->capacity; }

void queue_initialize(queue_t* queue, size_t capacity, size_t data_size) {
  queue = malloc(sizeof(queue_t));
  memset(queue, 0, sizeof(queue_t));
  queue->capacity = capacity;
  queue->data_size = data_size;
  queue->data = malloc(capacity * data_size);
}

void queue_destroy(queue_t* queue) {
  free(queue->data);
  free(queue);
}

int push(queue_t* queue, const void* item) {
  const size_t current_tail =
      atomic_load_explicit(&queue->tail, memory_order_relaxed);
  const size_t next_tail = queue_increment(queue, current_tail);
  if (next_tail != atomic_load_explicit(&queue->head, memory_order_acquire)) {
    memcpy(queue->data[current_tail], item, queue->data_size);
    atomic_store_explicit(&queue->tail, next_tail, memory_order_release);
    return 1;
  }
  return 0;  // full queue
}

int pop(queue_t* queue, void* item) {
  const size_t current_head =
      atomic_load_explicit(&queue->head, memory_order_relaxed);
  if (current_head == atomic_load_explicit(&queue->tail, memory_order_acquire))
    return 0;  // empty queue
  memcpy(&item, queue->data[current_head], queue->data_size);
  atomic_store_explicit(&queue->head, queue_increment(queue, current_head),
                        memory_order_release);
  return 1;
}

#endif
