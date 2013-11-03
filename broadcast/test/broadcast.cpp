#include "Framework/Timer.hpp"
#include "Framework/Broadcaster.hpp"

unsigned char* buffer;

struct App : Timer, Broadcaster {
  void onTimer() {
    for (int i = 0; i < packetSize; i++) buffer[i]++;
    send(buffer);
    printf("%03u\n", buffer[0]);
  }
};

int main(int argc, char* argv[]) {
  unsigned packetSize = 512;
  const char* ip = "127.0.0.1";
  unsigned port = 8888;
  if (argc > 1) packetSize = atoi(argv[1]);
  if (argc > 2) ip = argv[2];
  if (argc > 3) port = atoi(argv[2]);

  buffer = new unsigned char[packetSize];
  for (int i = 0; i < packetSize; i++) buffer[i] = (unsigned char)(i & 0xFF);

  App app;
  app.init(packetSize, ip, port);
  app.start(1.0f);
  getchar();
}
