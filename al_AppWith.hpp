#ifndef __AL_APP_WITH__
#define __AL_APP_WITH__

#include "allocore/io/al_App.hpp"
#include <unistd.h>
#include <string.h>
#include <cassert>

#define QUEUE_LENGTH (3)

template <typename PODTYPE>
struct AppWith : al::App {

  enum {
    READER,
    WRITER,
    LOCAL
  } mode;

  PODTYPE* queue[QUEUE_LENGTH];
  unsigned read_index, write_index;

  PODTYPE& data() {
    switch (mode) {
      case READER:
      case LOCAL:
        return *queue[read_index];
      case WRITER:
        return *queue[write_index];
    }
  }

  void init() {
    mode = LOCAL;
    std::cout << "this AppWith<State> is local" << std::endl;

    // XXX
    queue[0] = new PODTYPE();
  }

  void init(const char* writer) {
    char hostname[256];
    assert(gethostname(hostname, 256) >= 0);
    bool isWriter = (strncmp(writer, hostname, 256) == 0);
    mode = isWriter ? WRITER : READER;
    std::cout << "this AppWith<State> is "
              << (isWriter ? "the writer" : "a reader") << std::endl;

    std::cout << "hostname is " << hostname << std::endl;

    // XXX
    queue[0] = new PODTYPE();
  }

  void poll() {}
  void send() {}
};

#endif
