#include "Framework/HostRole.hpp"
#include "Framework/Selector.hpp"
#include "Framework/Broadcaster.hpp"
#include "Framework/Timer.hpp"
#include <openssl/sha.h>

void sha() {
  const unsigned char d[] = "Original String";
  unsigned char md[SHA_DIGEST_LENGTH];
  SHA1(d, sizeof(d) - 1, md);
}

struct App : HostRole, Selector<App>, Broadcaster, Timer {
  unsigned char* buffer;
  unsigned char* b;
  void init(unsigned packetSize, float rate, unsigned timeout, const char* ip,
            unsigned port) {
    buffer = new unsigned char[packetSize];
    b = new unsigned char[packetSize];
    HostRole::init();
    if (isSelector) Selector<App>::start(packetSize, timeout, port);
    if (isBroadcaster) {
      Broadcaster::init(packetSize, ip, port);
      Timer::start(rate);
    }
  }

  void onTimer() {
    send(b);
    b[0]++;
    printf("onTimer(): %03u\n", b[0]);
  }

  void onNewBuffer() { printf("onNewBuffer(): %03u\n", buffer[0]); }
};

int main(int argc, char* argv[]) {
  unsigned packetSize = 512;
  float rate = 0.8f;
  unsigned timeout = 900000;
  const char* ip = "127.0.0.1";
  unsigned port = 8888;
  if (argc > 1) packetSize = atoi(argv[1]);
  if (argc > 2) rate = atof(argv[2]);
  if (argc > 3) timeout = atoi(argv[3]);
  if (argc > 4) ip = argv[4];
  if (argc > 5) port = atoi(argv[5]);

  printf("packetSize:%u ", packetSize);
  printf("rate:%f ", rate);
  printf("timeout:%u ", timeout);
  printf("ip:%s ", ip);
  printf("port:%u ", port);
  printf("\n");

  App app;
  app.init(packetSize, rate, timeout, ip, port);
  getchar();
}
