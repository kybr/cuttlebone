#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lo/lo.h"
#include "common.hpp"

void error(int num, const char *msg, const char *path) {
  printf("liblo server error %d in path %s: %s\n", num, path, msg);
  fflush(stdout);
}

int generic_handler(const char *path, const char *types, lo_arg **argv,
                    int argc, void *data, void *user_data) {
  int i;

  printf("path: <%s>\n", path);
  for (i = 0; i < argc; i++) {
    printf("arg %d '%c' ", i, types[i]);
    lo_arg_pp((lo_type)types[i], argv[i]);
    printf("\n");
  }
  printf("\n");
  fflush(stdout);

  return 1;
}

struct Wrapper {
  int size;
  Foo foo;
};

int state_handler(const char *path, const char *types, lo_arg **argv, int argc,
                  void *data, void *user_data) {
  Wrapper *wrapper = (Wrapper *)argv[0];
  printf("%s <- size:%d | n:%d, name:%s\n", path, wrapper->size, wrapper->foo.n,
         wrapper->foo.name);
  fflush(stdout);
  return 0;
}

int main() {
  lo_server_thread st =
      lo_server_thread_new_multicast("224.0.0.1", "9999", error);
  lo_server_thread_add_method(st, "/state", "b", state_handler, NULL);
  lo_server_thread_add_method(st, NULL, NULL, generic_handler, NULL);
  lo_server_thread_start(st);

  while (true) {
    usleep(1000000);
  }

  return 0;
}

