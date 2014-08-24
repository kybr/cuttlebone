#include <stdio.h>  // getchar
#include "queue.h"
int main() {
  int foo[10];
  queue_t* q;
  queue_initialize(q, 5, sizeof(foo));
  foo[0] = 0;
  push(q, foo);
  foo[0]++;
  push(q, foo);
  foo[0]++;
  push(q, foo);
  getchar();
  pop(q, foo);
  pop(q, foo);
  pop(q, foo);
  queue_destroy(q);
}
