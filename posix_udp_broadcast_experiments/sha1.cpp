// c++ sha1.cpp -lcrypto
//
#include <openssl/sha.h>
int main() {
  const unsigned char d[] = "Original String";
  unsigned char md[SHA_DIGEST_LENGTH];
  SHA1(d, sizeof(d) - 1, md);
}
