#ifndef __CHECKSUM__
#define __CHECKSUM__

#include <openssl/sha.h>

struct Checksum {
  unsigned char md[SHA_DIGEST_LENGTH];
  void print() {
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) printf("%02x", md[i]);
    printf("\n");
  }
  void checksum(unsigned char* data, unsigned size) { SHA1(data, size, md); }
};

#endif