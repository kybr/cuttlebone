
#include "Stuff.hpp"
#include <openssl/sha.h>
#include <unistd.h>
#include <string.h>
#include <cassert>

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

  char message[256];
  Writer writer;
  Reader reader;

  writer.init();
  reader.init();

  unsigned n = 0;
  while (true) {
    sprintf(message, "%03u", n++);
    writer.send(message);
    reader.poll();
    sleep(1);
  }
}
