#ifndef __BROADCASTER__
#define __BROADCASTER__

#include <arpa/inet.h>  // struct sockaddr_in
#include <unistd.h>     // sleep()
#include <cstring>      // linux
#include <cstdlib>      // linux
#include <cstdio>       // linux
#include <string>
#include <thread>
#include <cassert>
#include "Cuttlebone/Log.hpp"

namespace cuttlebone {

struct Broadcaster {
  struct sockaddr_in address;
  int fileDescriptor;
  unsigned packetSize;

  void init(unsigned packetSize, const char* ip, int port,
            bool useLargeWindow = true) {

    this->packetSize = packetSize;

    assert((fileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) >= 0);

    int broadcast = 1;
    setsockopt(fileDescriptor, SOL_SOCKET, SO_BROADCAST, &broadcast,
               sizeof(broadcast));

    if (useLargeWindow) {
      int window = 16777216;
      if (setsockopt(fileDescriptor, SOL_SOCKET, SO_SNDBUF, &window,
                     sizeof(int)) == -1) {
        fprintf(stderr, "Error setting socket opts: %s\n", strerror(errno));
      }
      printf("%d byte send buffer (aka \"window\")\n", window);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);

    LOG("broadcasting to %s on port %d", ip, port);
  }

  void send(unsigned char* data) {
    if (sendto(fileDescriptor, data, packetSize, 0, (struct sockaddr*)&address,
               sizeof(address)) >= 0) {
    } else {
      LOG("failed to SEND packet");
    }
  }
};

}  // cuttlebone

#endif
