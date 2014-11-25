#include <arpa/inet.h>  // struct sockaddr_in
#include <unistd.h>     // sleep()
#include <cstring>      // linux
#include <cstdlib>      // linux
#include <cstdio>       // linux
#include <string>
#include <thread>
#include <cassert>
#include "Cuttlebone/Log.hpp"

int main(int argc, char* argv[]) {
  struct sockaddr_in address;
  int fileDescriptor;
  unsigned packetSize;

  const char* ip = argv[1];
  int port = atoi(argv[2]);

  fileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
  assert(fileDescriptor >= 0);

  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = inet_addr(ip);

  printf(" ---> %s:%d\n", ip, port);

  packetSize = 10000;
  char data[packetSize];
  while (sendto(fileDescriptor, data, packetSize, 0, (struct sockaddr*)&address,
                sizeof(address)) < 0) {
    packetSize /= 2;
    usleep(100);
  }

  while (sendto(fileDescriptor, data, packetSize, 0, (struct sockaddr*)&address,
                sizeof(address)) >= 0) {
    packetSize++;
    usleep(100);
  }

  printf("MAX:%u\n", packetSize);
}
