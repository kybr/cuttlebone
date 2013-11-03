#ifndef __SELECTOR__
#define __SELECTOR__

#include <arpa/inet.h>  // struct sockaddr_in
#include <unistd.h>     // sleep()
#include <cstring>      // linux
#include <cstdlib>      // linux
#include <cstdio>       // linux
#include <string>
#include <thread>
#include <cassert>

template <typename APP>
struct Selector {
  std::thread t;
  int packetSize, timeout, port;
  bool done;

  void start(int packetSize = 512, int timeout = 999999, int port = 8888) {
    this->packetSize = packetSize;
    this->timeout = timeout;
    this->port = port;
    t = std::thread(&Selector::init, this);
  }

  void init() {

    printf("packetSize:%u timeout:%u port:%u\n", packetSize, timeout, port);

    int fileDescriptor;
    if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
      perror("socket");
      exit(-1);
    }

    //int broadcast = 1;
    //if (setsockopt(fileDescriptor, SOL_SOCKET, SO_BROADCAST, &broadcast,
    //               sizeof(broadcast)) == -1) {
    //  perror("setsockopt");
    //  exit(-1);
    //}

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(fileDescriptor, (sockaddr*)&address, sizeof(sockaddr)) == -1) {
      perror("bind");
      exit(-1);
    }

    while (!done) {
      fd_set fileDescriptorSet;
      FD_ZERO(&fileDescriptorSet);
      FD_SET(fileDescriptor, &fileDescriptorSet);

      struct timeval tv = {0, timeout};  // sec, usec
      //printf("BEFORE: %ld, %ld\n", tv.tv_sec, tv.tv_usec);
      int rv = select(fileDescriptor + 1, &fileDescriptorSet, 0, 0, &tv);
      //printf("AFTER: %ld, %ld\n", tv.tv_sec, tv.tv_usec);

      // XXX so is tv now the actual time that select waited?

      if (rv == -1) {
        perror("select");
      } else if (rv == 0) {
        printf("Timeout! Who cares?\n");
      } else {
        int bytesReceived =
            recvfrom(fileDescriptor, static_cast<APP*>(this)->buffer,
                     packetSize, 0, 0, 0);
        if (bytesReceived == -1) {
          perror("recvfrom");
        } else if (bytesReceived != packetSize) {
          printf("Received less than expected\n");
        } else {
          static_cast<APP*>(this)->onNewBuffer();
        }
      }
    }
  }

  void stop() {
    done = true;
    t.join();
  }
};

#endif
