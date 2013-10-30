#include "Stuff.hpp"
#include <cassert>

#define N (1024)

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
    usleep(400000);
    reader.poll(received);
    printf(" %03u\n", received[0]);
    usleep(400000);
  }
}
