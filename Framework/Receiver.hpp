#ifndef __SELECTOR__
#define __SELECTOR__

#include <sys/select.h>  // select
#include <arpa/inet.h>   // struct sockaddr_in
#include <cstring>       // linux
#include <cstdlib>       // linux
#include <cstdio>        // linux
#include <string>
#include <cassert>

struct Receiver {
  int fileDescriptor;

  void init(unsigned port) {

    if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
      perror("socket");
      exit(-1);
    }

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(fileDescriptor, (sockaddr*)&address, sizeof(sockaddr)) == -1) {
      perror("bind");
      exit(-1);
    }
  }

  bool receive(void* buffer, unsigned packetSize, unsigned timeout) {
    fd_set fileDescriptorSet;
    FD_ZERO(&fileDescriptorSet);
    FD_SET(fileDescriptor, &fileDescriptorSet);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout;

    // printf("BEFORE: %ld, %ld\n", tv.tv_sec, tv.tv_usec);
    int rv = select(fileDescriptor + 1, &fileDescriptorSet, 0, 0, &tv);
    // printf("AFTER: %ld, %ld\n", tv.tv_sec, tv.tv_usec);

    // XXX so is tv now the actual time that select waited?

    if (rv == -1) {
      perror("select");
      return false;
    } else if (rv == 0) {
      printf("Timeout! Who cares?\n");
      return false;
    } else {
      int bytesReceived = recvfrom(fileDescriptor, buffer, packetSize, 0, 0, 0);
      if (bytesReceived == -1) {
        perror("recvfrom");
        return false;
      } else if (bytesReceived != packetSize) {
        printf("Received less than expected\n");
        return false;
      } else {
        return true;
      }
    }
  }
};

#endif
