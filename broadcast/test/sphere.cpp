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
  void init() {
    buffer = new unsigned char[512];
    HostRole::init();
    if (isBroadcaster) {
      Broadcaster::init();
      Timer::start(0.8);
    }
    if (isSelector) Selector<App>::start();
  }

  void onTimer() {
    static unsigned char b[512];
    send(b);
    b[0]++;
    printf("onTimer(): %03u\n", b[0]);
  }

  void onNewBuffer() { printf("onNewBuffer(): %03u\n", buffer[0]); }
};

int main() {
  App app;
  app.init();
  getchar();
}
