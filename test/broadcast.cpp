#include "Framework/Timer.hpp"
#include "Framework/Broadcaster.hpp"
#include "Framework/Checksum.hpp"

unsigned char* buffer;

struct App : Timer, Broadcaster, Checksum {
  void onTimer() {
    checksum(buffer, packetSize);
    //    printf("\033[u\033[2K\033[s");
    printf("%03u | ", buffer[0]);
    print();
    printf("\n");
    fflush(stdout);
    send(buffer);
    for (unsigned i = 0; i < packetSize; i++) buffer[i]++;
  }
};

int main(int argc, char* argv[]) {
  unsigned packetSize = 1024;
  float timerPeriod = 0.8f;
  const char* ip = "127.0.0.1";
  unsigned port = 8888;
  if (argc > 1) packetSize = atoi(argv[1]);
  if (argc > 2) timerPeriod = atof(argv[2]);
  if (argc > 3) ip = argv[3];
  if (argc > 4) port = atoi(argv[4]);

  printf("usage: %s packetSize timerPeriod ip port\n", argv[0]);
  printf("broadcasting %u bytes to %s:%u every %f seconds\n", packetSize, ip,
         port, timerPeriod);
  printf("#   | sha1 sum\n");

  buffer = new unsigned char[packetSize];
  for (unsigned i = 0; i < packetSize; i++)
    buffer[i] = (unsigned char)(i & 0xFF);

  App app;
  app.init(packetSize, ip, port);
  app.start(timerPeriod);
  getchar();
}
