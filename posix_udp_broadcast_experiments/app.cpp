#include "Stuff.hpp"
#include <cassert>

int main() {
  unsigned char message[256];
  Writer<256> writer;
  Reader<256> reader;

  writer.init();
  reader.init();

  unsigned n = 0;
  while (true) {
    for (int i = 0; i < 256; i++)
      message[i]++;
    writer.send(message);
    reader.poll(message);
    printf("%03u\n", message[0]);
    sleep(1);
  }
}
