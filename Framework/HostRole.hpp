#ifndef __HOSTROLE__
#define __HOSTROLE__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    printf("i am %s\n", hostname);

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

    if (isSimulator) printf("i am the simulator\n");
    if (isGraphicsRenderer) printf("i am a graphics renderer\n");
    if (isAudioRenderer) printf("i am an audio renderer\n");
  }

  const char* broadcastIpAddress() {
    if (isSimulator && isGraphicsRenderer && isAudioRenderer)
      // we're running on a laptop. use the "home" ip address
      return "127.0.0.1";
    else
      return config[0];
  }
};

#endif
