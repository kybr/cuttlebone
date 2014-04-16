#ifndef __CHECKSUM__
#define __CHECKSUM__

#include <openssl/sha.h>
#include <stdio.h>

struct Checksum {
  unsigned char data[SHA_DIGEST_LENGTH];

  void print() {
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) printf("%02x", data[i]);
  }

  void checksum(unsigned char* data, unsigned size) {
    SHA1(data, size, this->data);
  }
};

#endif