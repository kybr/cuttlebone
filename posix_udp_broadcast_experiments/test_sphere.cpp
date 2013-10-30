#include "Stuff.hpp"
#include <openssl/sha.h>
#include <unistd.h>
#include <string.h>
#include <cassert>

#define N (256)

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

  unsigned char sent[N];
  unsigned char received[N];

  if (isWriter) {
    Writer<N> writer;
    writer.init("192.168.0.255");

    while (true) {
      writer.send(sent);
      printf("%03u", sent[0]);
      for (int i = 0; i < N; i++) sent[i]++;
      usleep(wait_time);
    }
  } else {
    Reader<N> reader;
    reader.init();
    usleep(wait_time);
    while (true) {
      reader.poll(received);
      printf("%03u", received[0]);
      usleep(wait_time);
    }
  }
}
