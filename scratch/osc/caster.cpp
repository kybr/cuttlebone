#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lo/lo.h"
#include "common.hpp"

int main(int argc, char *argv[]) {
  Foo foo;
  foo.n = 0;
  lo_address t = lo_address_new_with_proto(LO_UDP, "224.0.0.1", "9999");
  while (true) {
    sprintf(foo.name, "%d", foo.n);
    lo_blob b = lo_blob_new(sizeof(Foo), &foo);
    if (lo_send(t, "/state", "b", b) == -1)
      printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
    lo_blob_free(b);
    foo.n++;
    usleep(1000000);
  }
  return 0;
}
