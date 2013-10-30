#include "Stuff.hpp"
#include <cassert>
#include <thread>
using namespace std;

#define N (1024)
#define WAIT (100000)

int main() {
  unsigned char sent[N];
  Writer<N> writer;
  writer.init();
  for (int i = 0; i < N; i++) sent[i] = i;
  unsigned n = 0;

  thread foo([]() {
      unsigned char received[N];
      Reader<N> reader;
      reader.init();
      for (int k = 0; k < 10; k++) {
        reader.poll(received);
        printf(" %03u\n", received[0]);
        usleep(WAIT);
    }
  });

  for (int k = 0; k < 10; k++) {
    writer.send(sent);
    printf("%03u ->", sent[0]);
    for (int i = 0; i < N; i++) sent[i]++;
    usleep(WAIT);
  }

  foo.join();
}
