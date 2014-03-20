// this should serve as a central point of configuration for broadcast apps.
// right now, broadcast address and host configuration (simulator, audio,
// visual, device) is supported here. but in the future, node geometries may be
// kept here in support of spatiallized sound and graphics.
//

#ifndef __HOSTROLE__
#define __HOSTROLE__

#include <stdlib.h>  // perror
#include <string.h>  // strncmp
#include "Framework/Log.hpp"

struct ConfigurationData {
  const char* name;
  const char* broadcast;
  bool simulation, audio, visual, device;
};

ConfigurationData defaultConfigurationData[] = {
    {"gr01", .simulation = true, .broadcast = "192.168.10.255"},
    {"audio", .audio = true},
    {"gr02", .visual = true},
    {"gr03", .visual = true},
    {"gr04", .visual = true},
    {"gr05", .visual = true},
    {"gr06", .visual = true},
    {"gr07", .visual = true},
    {"gr08", .visual = true},
    {"gr09", .visual = true},
    {"gr10", .visual = true},
    {"gr11", .visual = true},
    {"gr12", .visual = true},
    {"gr13", .visual = true},
    {"gr14", .visual = true}, };

struct Configuration : ConfigurationData {

  Configuration() : Configuration(defaultConfigurationData) {
    LOG("using default configuration");
  }

  template <size_t N>
  Configuration(ConfigurationData (&configurationCandidate)[N]) {

    char hostname[256];
    if (gethostname(hostname, 256) == -1) {
      perror("gethostname");
      exit(-1);
    }

    int self = -1;
    bool foundSelf = false;
    for (int i = 0; i < N; ++i)
      if (strncmp(configurationCandidate[i].name, hostname, 256) == 0) {
        foundSelf = true;
        self = i;
        break;
      }

    if (foundSelf) {
      name = configurationCandidate[self].name;
      simulation = configurationCandidate[self].simulation;
      audio = configurationCandidate[self].audio;
      visual = configurationCandidate[self].visual;
      device = configurationCandidate[self].device;
      broadcast = configurationCandidate[self].broadcast;

      LOG("i am %s; i do%s%s%s%s.", hostname, simulation ? " simulation" : "",
          audio ? " audio" : "", visual ? " visual" : "",
          device ? " device" : "");
    } else {
      LOG("running in \"laptop mode\".");
      name = hostname;
      simulation = true;
      audio = true;
      visual = true;
      device = true;
    }
  }
};

#endif
