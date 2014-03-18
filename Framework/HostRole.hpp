#ifndef __HOSTROLE__
#define __HOSTROLE__

//#include <unistd.h>
//#include <stdio.h>
#include <stdlib.h> // perror
#include <string.h> // strncmp
#include "Framework/Log.hpp"

const char* config[] = {
  "192.168.10.255", "gr01", "audio", "gr02", "gr03", "gr04", "gr05", "gr06",
  "gr07", "gr08", "gr09", "gr10", "gr11",  "gr12", "gr13", "gr14",
};

struct HostRole {
  bool isSimulator, isGraphicsRenderer, isAudioRenderer;
  char hostname[256];

  void init() {
    if (gethostname(hostname, 256) == -1) {
      perror("gethostname");
      exit(-1);
    }

    //printf("i am %s\n", hostname);
    LOG("i am %s", hostname);

    if (strncmp(config[1], hostname, 256) == 0) {
      isSimulator = true;
      isGraphicsRenderer = false;
      isAudioRenderer = false;
    } else if (strncmp(config[2], hostname, 256) == 0) {
      isSimulator = false;
      isGraphicsRenderer = false;
      isAudioRenderer = true;
    } else {
      bool foundHostname = false;
      for (unsigned i = 3; i < (sizeof(config) / sizeof(config[0])); i++)
        if (strncmp(config[i], hostname, 256) == 0) {
          foundHostname = true;
        }
      if (foundHostname) {
        isSimulator = false;
        isGraphicsRenderer = true;
        isAudioRenderer = false;
      } else {
        isSimulator = true;
        isGraphicsRenderer = true;
        isAudioRenderer = true;
      }
    }

    if (isSimulator) LOG("i am the simulator");
    if (isGraphicsRenderer) LOG("i am a graphics renderer");
    if (isAudioRenderer) LOG("i am an audio renderer");
  }

  const char* broadcastIpAddress() {
    if (isSimulator && isGraphicsRenderer && isAudioRenderer)
      return "127.0.0.1"; // we're running on a laptop. use the "home" ip address
    else
      return config[0];
  }
};

#endif
