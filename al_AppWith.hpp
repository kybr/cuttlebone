#ifndef __AL_APP_WITH__
#define __AL_APP_WITH__

#include "allocore/io/al_App.hpp"
#include <unistd.h>
#include <string.h>
#include <cassert>

#define QUEUE_LENGTH (3)

template <typename PODTYPE>
struct AppWith : al::App {
  PODTYPE* queue[QUEUE_LENGTH];
  bool isWriter, local;
  unsigned read, write;

  AppWith() : read(0), write(1), isWriter(false) {}

  PODTYPE& data() { return *queue[0]; }

  void init() {
    local = true;

    std::cout << "this AppWith<State> is local" << std::endl;

    // XXX
    queue[0] = new PODTYPE();
  }

  void init(const char* writer) {
    local = false;

    char hostname[256];
    assert(gethostname(hostname, 256) >= 0);
    isWriter = (strncmp(writer, hostname, 256) == 0);

    std::cout << "this AppWith<State> is "
              << (isWriter ? "the writer" : "a reader") << std::endl;

    std::cout << "hostname is " << hostname << std::endl;

    // XXX
    queue[0] = new PODTYPE();
  }

  void poll() {}
};

#endif
