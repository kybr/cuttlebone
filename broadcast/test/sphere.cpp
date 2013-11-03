#include "Framework/Broadcaster.hpp"
#include "Framework/Selector.hpp"
#include "Framework/HostRole.hpp"
#include <openssl/sha.h>

void sha() {
  const unsigned char d[] = "Original String";
  unsigned char md[SHA_DIGEST_LENGTH];
  SHA1(d, sizeof(d) - 1, md);
}

struct App : HostRole {
};

int main() {
  App app;
  app.init();
  getchar();
}
