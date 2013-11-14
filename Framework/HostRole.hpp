#ifndef __HOSTROLE__
#define __HOSTROLE__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* data[] = {"gr11", "gr01", "gr03", "gr04", "gr05", "gr06", "gr07",
                      "gr08", "gr09", "gr10", "gr12", "gr13", "gr14", };

struct HostRole {
  bool isBroadcaster;
  bool isSelector;
  char hostname[256];

  void init() {
    if (gethostname(hostname, 256) == -1) {
      perror("gethostname");
      exit(-1);
    }

    printf("i am %s\n", hostname);

    if (strncmp(data[0], hostname, 256) == 0) {
      isBroadcaster = true;
      isSelector = false;
    } else {
      bool foundHostname = false;
      for (unsigned i = 1; i < (sizeof(data) / sizeof(data[0])); i++)
        if (strncmp(data[i], hostname, 256) == 0) foundHostname = true;
      if (foundHostname) {
        isBroadcaster = false;
        isSelector = true;
      } else {
        isBroadcaster = true;
        isSelector = true;
      }
    }

    if (isBroadcaster) printf("i am a broadcaster\n");
    if (isSelector) printf("i am a selector\n");
  }
};

#endif
