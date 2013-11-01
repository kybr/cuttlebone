#include <sys/select.h>  // select
#include <sys/socket.h>  // recvfrom
#include <cstdio>        // perror
#include <arpa/inet.h>   // struct sockaddr_in
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {

  int packetSize = 512, timeout = 999999, port = 8888;

  if (argc > 1) packetSize = atoi(argv[1]);
  if (argc > 2) timeout = atoi(argv[2]);
  if (argc > 3) port = atoi(argv[3]);

  printf("packetSize:%u timeout:%u port:%u\n", packetSize, timeout, port);

  struct timeval tv = {0, timeout};  // sec, usec

  unsigned char buffer[packetSize];

  int fileDescriptor;
  if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("socket");
    exit(-1);
  }

  int broadcast = 1;
  if (setsockopt(fileDescriptor, SOL_SOCKET, SO_BROADCAST, &broadcast,
                 sizeof(broadcast)) == -1) {
    perror("setsockopt");
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

  while (true) {
    fd_set fileDescriptorSet;
    FD_ZERO(&fileDescriptorSet);
    FD_SET(fileDescriptor, &fileDescriptorSet);

    int rv = select(fileDescriptor + 1, &fileDescriptorSet, 0, 0, &tv);
    if (rv == -1) {
      perror("select");
    } else if (rv == 0) {
      printf("Timeout!\n");
    } else {
      int bytesReceived = recvfrom(fileDescriptor, buffer, packetSize, 0, 0, 0);
      if (bytesReceived == -1) {
        perror("recvfrom");
      } else if (bytesReceived != packetSize) {
        printf("Received less than expected\n");
      } else {
        printf("%03u\n", buffer[0]);
      }
    }
  }
}
