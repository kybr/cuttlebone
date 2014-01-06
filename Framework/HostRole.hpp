#ifndef __HOSTROLE__
#define __HOSTROLE__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct HostRole {
  bool isBroadcaster;
  bool isSelector;
  char hostname[256];

  void init(const char** hostRole) {
    if (gethostname(hostname, 256) == -1) {
      perror("gethostname");
      exit(-1);
    }

    printf("i am %s\n", hostname);

    if (strncmp(hostRole[0], hostname, 256) == 0) {
      isBroadcaster = true;
      isSelector = false;
    } else {
      bool foundHostname = false;
      for (unsigned i = 1; i < (sizeof(hostRole) / sizeof(hostRole[0])); i++)
        if (strncmp(hostRole[i], hostname, 256) == 0) foundHostname = true;
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
