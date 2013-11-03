#include "Framework/Selector.hpp"
#include <iostream>
using namespace std;

struct App : Selector<App> {
  unsigned char* buffer;
  void onNewBuffer() {
    cout << "onNewBuffer(): " << (unsigned)buffer[0] << endl;
  }
};

int main(int argc, char* argv[]) {
  unsigned packetSize = 512;
  unsigned timeout = 999999;
  unsigned port = 8888;
  if (argc > 1) packetSize = atoi(argv[1]);
  if (argc > 2) timeout = atoi(argv[2]);
  if (argc > 3) port = atoi(argv[3]);

  App app;
  app.buffer = new unsigned char[packetSize];
  app.start(packetSize, timeout, port);
  getchar();
  app.stop();
}
