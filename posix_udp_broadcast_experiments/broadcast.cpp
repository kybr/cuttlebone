#include "Stuff.hpp"
#include <cassert>
using namespace std;

#define N (7000)
#define WAIT (100000)

int main() {
  unsigned char sent[N];
  Writer<N> writer;
  writer.init();
  for (int i = 0; i < N; i++) sent[i] = i;
  unsigned n = 0;

  while (true) {
    writer.send(sent);
    for (int i = 0; i < N; i++) sent[i]++;
    usleep(WAIT);
  }
}
