#ifndef __STUFF__
#define __STUFF__

#include <stdlib.h>     // exit()
#include <arpa/inet.h>  // struct sockaddr_in
#include <unistd.h>     // sleep()
#include <string>

struct Writer {
  struct sockaddr_in address;
  int s;

  void init() {
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("socket");
      exit(1);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8888);
  }

  void send(std::string message) {
    if (sendto(s, message.c_str(), message.size(), 0,
               (struct sockaddr *)&address, sizeof(address)) < 0) {
      perror("sendto");
      exit(1);
    }
  }
};

struct Reader {
  void init() {}
  void poll() {}
};

#endif
