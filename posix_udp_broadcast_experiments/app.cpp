#include "Stuff.hpp"
#include <cassert>
#include <thread>

#define N (1024)
#define WAIT (100000)

int main() {
  unsigned char sent[N];
  unsigned char received[N];
  Writer<N> writer;
  Reader<N> reader;

  writer.init();
  reader.init();

  for (int i = 0; i < N; i++)
    sent[i] = i;

  unsigned n = 0;
  while (true) {
    writer.send(sent);
    printf("%03u ->", sent[0]);
    for (int i = 0; i < N; i++)
      sent[i]++;
    usleep(WAIT);
    reader.poll(received);
    printf(" %03u\n", received[0]);
    usleep(WAIT);
  }
}
