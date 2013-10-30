#include "Stuff.hpp"
#include <openssl/sha.h>
#include <unistd.h>
#include <string.h>
#include <cassert>
#include <signal.h>

int wait_time = 100000;

static bool done = false;
void quit(int) {
  done = true;
}

int sha() {
  const unsigned char d[] = "Original String";
  unsigned char md[SHA_DIGEST_LENGTH];
  SHA1(d, sizeof(d) - 1, md);
}

int main() {
  signal(SIGINT, quit);

  char hostname[256];
  assert(gethostname(hostname, 256) >= 0);
  printf("i am %s\n", hostname);
  bool isWriter = (strncmp("photon", hostname, 256) == 0);
  printf("i am %s\n", isWriter ? "the writer" : "a reader");
  unsigned char message[256];


  if (isWriter) {
    Writer<256> writer;
    writer.init("192.168.0.255");

    while (!done) {
      writer.send(message);
      printf("%03u", message[0]);
      for (int i = 0; i < 256; i++)
        message[i]++;
      usleep(wait_time);
    }
  } else {
    Reader<256> reader;
    reader.init();
    while (!done) {
      reader.poll(message);
      printf("%03u", message[0]);
      usleep(wait_time);
    }
  }
}
