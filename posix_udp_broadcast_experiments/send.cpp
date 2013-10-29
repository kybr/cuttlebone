// queue cases
//   single reader, single writer
//   with a greedy reader
//   writer.. write on A
//   reader.. 


#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  struct sockaddr_in address;
  int s;
  const char *message = "Hello, World!";

  if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.255");
  address.sin_port = htons(8888);

  while (1) {
    if (sendto(s, message, strlen(message), 0, (struct sockaddr *)&address,
               sizeof(address)) < 0) {
      perror("sendto");
      exit(1);
    }
    printf("GOT HERE\n");
    sleep(3);
  }
}
