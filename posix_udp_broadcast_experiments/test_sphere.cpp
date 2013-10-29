#include "Stuff.hpp"
#include <openssl/sha.h>
#include <unistd.h>
#include <string.h>
#include <cassert>

int wait_time = 100000;

int sha() {
  const unsigned char d[] = "Original String";
  unsigned char md[SHA_DIGEST_LENGTH];
  SHA1(d, sizeof(d) - 1, md);
}

int main() {
  char hostname[256];
  assert(gethostname(hostname, 256) >= 0);
  printf("i am %s\n", hostname);
  bool isWriter = (strncmp("photon", hostname, 256) == 0);
  printf("i am %s\n", isWriter ? "the writer" : "a reader");

  if (isWriter) {
    char message[256];
    Writer writer;
    writer.init("192.168.0.255");

    unsigned n = 0;
    while (true) {
      sprintf(message, "%03u", n++);
      writer.send(message);
      usleep(wait_time);
    }
  } else {
    Reader reader;
    reader.init();
    while (true) {
      reader.poll();
      usleep(wait_time);
    }
  }
}
