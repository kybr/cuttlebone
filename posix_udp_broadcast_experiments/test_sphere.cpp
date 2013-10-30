#include "Stuff.hpp"
#include <openssl/sha.h>
#include <unistd.h>
#include <string.h>
#include <cassert>

#define N (1024)

int wait_time = 500000;

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
    unsigned char sent[N];
    Writer<N> writer;
    writer.init("192.168.0.255");
    for (int i = 0; i < N; i++) sent[i] = i;

    while (true) {
      writer.send(sent);
      printf("%03u\n", sent[0]);
      for (int i = 0; i < N; i++) sent[i]++;
      usleep(wait_time);
    }
  } else {
    unsigned char received[N];
    Reader<N> reader;
    reader.init();
    while (true) {
      reader.poll(received);
      printf("%03u\n", received[0]);
      usleep(wait_time);
    }
  }
}
