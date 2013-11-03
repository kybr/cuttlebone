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

struct Broadcaster {
  struct sockaddr_in address;
  int s;
  unsigned packetSize;

  void init(unsigned packetSize = 512, const char* ip = "127.0.0.1",
            int port = 8888) {
    this->packetSize = packetSize;

    assert((s = socket(AF_INET, SOCK_DGRAM, 0)) >= 0);

    int broadcast = 1;
    setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
  }

  void send(unsigned char* data) {
    assert(sendto(s, data, packetSize, 0, (struct sockaddr*)&address,
                  sizeof(address)) >= 0);
  }
};

#endif
