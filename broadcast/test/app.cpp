#include "Framework/HostRole.hpp"
#include "Framework/Selector.hpp"
#include "Framework/Broadcaster.hpp"
#include "Framework/Timer.hpp"
#include <openssl/sha.h>

struct Checksum {
  unsigned char md[SHA_DIGEST_LENGTH];
  void print() {
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) printf("%02x", md[i]);
    printf("\n");
  }
  void checksum(unsigned char* data, unsigned size) { SHA1(data, size, md); }
};

struct App : HostRole, Selector<App>, Broadcaster, Timer, Checksum {
  unsigned char* buffer;
  unsigned char* b;
  unsigned packetSize;
  void init(unsigned packetSize, float rate, unsigned timeout, const char* ip,
            unsigned port) {
    this->packetSize = packetSize;
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
    checksum(b, packetSize);
    print();
    send(b);
    b[0]++;
    printf("onTimer(): %03u\n", b[0]);
  }

  void onNewBuffer() {
    checksum(buffer, packetSize);
    print();
    printf("onNewBuffer(): %03u\n", buffer[0]);
  }
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
