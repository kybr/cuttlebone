#include "Framework/Timer.hpp"
#include "Framework/Broadcaster.hpp"

unsigned char* buffer;

struct App : Timer, Broadcaster {
  void onTimer() {
    for (unsigned i = 0; i < packetSize; i++) buffer[i]++;
    send(buffer);
    printf("%03u\n", buffer[0]);
  }
};

int main(int argc, char* argv[]) {
  unsigned packetSize = 512;
  float rate = 0.8f;
  const char* ip = "127.0.0.1";
  unsigned port = 8888;
  if (argc > 1) packetSize = atoi(argv[1]);
  if (argc > 2) rate = atof(argv[2]);
  if (argc > 3) ip = argv[3];
  if (argc > 4) port = atoi(argv[4]);

  printf("[%u] -> %s:%u @ %f\n", packetSize, ip, port, rate);

  buffer = new unsigned char[packetSize];
  for (unsigned i = 0; i < packetSize; i++) buffer[i] = (unsigned char)(i & 0xFF);

  App app;
  app.init(packetSize, ip, port);
  app.start(rate);
  getchar();
}
