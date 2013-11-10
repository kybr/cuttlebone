#include "Framework/Timer.hpp"
#include <stdio.h>

unsigned char* buffer;

struct App : Timer {
  void onTimer() {
    printf("this happened\n");
  }
};

int main(int argc, char* argv[]) {
  unsigned timerPeriod = 800000;
  if (argc > 1) timerPeriod = atof(argv[1]);

  printf("usage: %s timerPeriod\n", argv[0]);

  App app;
  app.start(timerPeriod);
  getchar();
}
