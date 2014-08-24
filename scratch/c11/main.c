#include <stdio.h>  // getchar
#include "queue.h"
int main() {
  data_t foo;
  queue_t* q = queue_initialize(5, sizeof(foo));
  queue_print(q);
  foo.n[0] = 0;
  printf("%d ", foo.n[0]);
  push(q, &foo);
  foo.n[0]++;
  printf("%d ", foo.n[0]);
  push(q, &foo);
  foo.n[0]++;
  printf("%d ", foo.n[0]);
  push(q, &foo);
  printf("\n");
  getchar();
  pop(q, &foo);
  printf("%d ", foo.n[0]);
  pop(q, &foo);
  printf("%d ", foo.n[0]);
  pop(q, &foo);
  printf("%d ", foo.n[0]);
  printf("\n");
  queue_destroy(q);
}
