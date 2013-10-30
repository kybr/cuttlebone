#ifndef __STUFF__
#define __STUFF__

#include <arpa/inet.h>  // struct sockaddr_in
#include <unistd.h>     // sleep()
#include <cstring>      // linux
#include <cstdlib>      // linux
#include <cstdio>       // linux
#include <string>
#include <cassert>

template <int N>
struct Writer {
  struct sockaddr_in address;
  int s;

  void init(const char* ip = "127.0.0.1", int port = 8888) {
    assert((s = socket(AF_INET, SOCK_DGRAM, 0)) >= 0);

    int broadcast = 1;
    setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
  }

  void send(unsigned char* data) {
    assert(sendto(s, data, N, 0, (struct sockaddr*)&address, sizeof(address)) >=
           0);
  }
};

template <int N>
struct Reader {
  int s;

  void init(int port = 8888) {
    assert((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) >= 0);

    int broadcast = 1;
    setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    assert(bind(s, (sockaddr*)&address, sizeof(sockaddr)) >= 0);
  }

  void poll(unsigned char* buffer) {
    assert(recvfrom(s, buffer, N, 0, 0, 0) >= 0);
  }
};

#endif
