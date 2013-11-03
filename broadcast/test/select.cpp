#include "Framework/Selector.hpp"
#include "Framework/Checksum.hpp"
#include <iostream>
using namespace std;

struct App : Selector<App>, Checksum {
  unsigned char* buffer;
  void onNewBuffer() {
    checksum(buffer, packetSize);
    printf("%03u | ", buffer[0]);
    print();
    printf("\n");
    fflush(stdout);
  }
};

int main(int argc, char* argv[]) {
  unsigned packetSize = 1024;
  unsigned selectTimeout = 810000;
  unsigned port = 8888;
  if (argc > 1) packetSize = atoi(argv[1]);
  if (argc > 2) selectTimeout = atoi(argv[2]);
  if (argc > 3) port = atoi(argv[3]);

  printf("usage: %s packetSize selectTimeout port\n", argv[0]);
  printf("selecting %u bytes from port %u with %uus timeout\n", packetSize, port, selectTimeout);
  printf("#   | sha1 sum\n");

  App app;
  app.buffer = new unsigned char[packetSize];
  app.start(packetSize, selectTimeout, port);
  getchar();
  app.stop();
}
