#include "Stuff.hpp"

int main() {
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
