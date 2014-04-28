#include "Cuttlebone/Receiver.hpp"
#include "Cuttlebone/Checksum.hpp"
#include <unistd.h>  // sleep()

unsigned char* buffer;

int main(int argc, char* argv[]) {
  unsigned packetSize = 1024;
  float timeOut = 0.8f;
  unsigned port = 8888;
  if (argc > 1) packetSize = atoi(argv[1]);
  if (argc > 2) timeOut = atof(argv[2]);
  if (argc > 3) port = atoi(argv[3]);

  printf("usage: %s packetSize timeOut port\n", argv[0]);
  printf("receiving %u bytes from on port %u every %f seconds\n", packetSize,
         port, timeOut);
  printf("#   | sha1 sum\n");

  Checksum c;
  Receiver receiver;
  receiver.init(port);
  unsigned char buffer[packetSize];

  while (true) {
    if (receiver.receive(buffer, packetSize, timeOut)) {
      c.checksum(buffer, packetSize);
      printf("%03u | ", buffer[0]);
      c.print();
      printf("\n");
    }
  }
}
