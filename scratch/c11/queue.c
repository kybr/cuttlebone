#include "queue.h"
#include "stdatomic.h"
#include <stdlib.h> // size_t
#include <string.h>  // memcpy
#include <stdio.h>  // printf

typedef struct q {
  size_t capacity, data_size;
  atomic_size_t head, tail;
  data_t* data;
} queue_t;

size_t queue_increment(queue_t* q, size_t i) { return (i + 1) & (q->capacity - 1); }

size_t nlpo2(size_t x) {
  x--;
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  return (x+1);
} 

void queue_print(queue_t* queue) {
  printf("capacity:%lu data_size:%lu\n", queue->capacity, queue->data_size);
}

queue_t* queue_initialize(size_t capacity, size_t data_size) {
  queue_t* queue = malloc(sizeof(queue_t));
  memset(queue, 0, sizeof(queue_t));
  queue->capacity = nlpo2(capacity);
  queue->data_size = sizeof(data_t);
  //queue->data_size = data_size;
  queue->data = malloc(capacity * data_size);
  return queue;
}

void queue_destroy(queue_t* queue) {
  free(queue->data);
  free(queue);
}

int push(queue_t* queue, const data_t* item) {
  const size_t current_tail =
      atomic_load_explicit(&queue->tail, memory_order_relaxed);
  const size_t next_tail = queue_increment(queue, current_tail);
  if (next_tail != atomic_load_explicit(&queue->head, memory_order_acquire)) {
    memcpy(&queue->data[current_tail], item, queue->data_size);
    atomic_store_explicit(&queue->tail, next_tail, memory_order_release);
    return 1;
  }
  return 0;  // full queue
}

int pop(queue_t* queue, data_t* item) {
  const size_t current_head =
      atomic_load_explicit(&queue->head, memory_order_relaxed);
  if (current_head == atomic_load_explicit(&queue->tail, memory_order_acquire))
    return 0;  // empty queue
  memcpy(item, &queue->data[current_head], queue->data_size);
  atomic_store_explicit(&queue->head, queue_increment(queue, current_head),
                        memory_order_release);
  return 1;
}

