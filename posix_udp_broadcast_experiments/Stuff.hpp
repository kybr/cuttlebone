#ifndef __STUFF__
#define __STUFF__

#include <arpa/inet.h>  // struct sockaddr_in
#include <unistd.h>     // sleep()
#include <cstring>      // linux
#include <cstdlib>      // linux
#include <cstdio>       // linux
#include <string>

struct Writer {
  struct sockaddr_in address;
  int s;

  void init(const char* ip = "127.0.0.1", int port = 8888) {
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("socket");
      exit(1);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);
  }

  void send(std::string message) {
    if (sendto(s, message.c_str(), message.size(), 0,
               (struct sockaddr *)&address, sizeof(address)) < 0) {
      perror("sendto");
      exit(1);
    }
    printf("sent: %s\n", message.c_str());
  }
};

struct Reader {
  struct sockaddr_in address;
  int s;

  void init(int port = 8888) {
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("socket");
      exit(1);
    }

    int broadcast = 1;
    setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (sockaddr *)&address, sizeof(sockaddr)) < 0) {
      perror("bind");
      exit(1);
    }
  }

  void poll() {
    char buf[256];
    int n;
    if ((n = recvfrom(s, buf, sizeof(buf) - 1, 0, 0, 0)) < 0) {
      perror("recvfrom");
      exit(1);
    }
    buf[n] = '\0';
    printf("received: %s\n", buf);
  }
};

#endif
