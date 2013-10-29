// queue cases
//   single reader, single writer
//   with a greedy reader
//   writer.. write on A
//   reader..

#include <stdlib.h>
#include <sys/socket.h>  //
                         //#include <sys/types.h>
                         //#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
//#include <string.h>
#include <stdio.h>
#include <unistd.h>

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
    address.sin_addr.s_addr = inet_addr("127.0.0.255");
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

struct Reader {};

int main() {
  const char *message = "Hello, World!";
  Writer writer;
  writer.init();
  Reader reader;
  reader.init();

  while (true) {
    writer.send(message);
    reader.poll();
    printf("GOT HERE\n");
    sleep(1);
  }
}
