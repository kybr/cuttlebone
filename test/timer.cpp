#include "Framework/Timer.hpp"
#include <stdio.h>

unsigned char* buffer;

struct App : Timer {
  void onTimer() {
    printf("this happened\n");
  }
};

int main(int argc, char* argv[]) {
  float timerPeriod = 0.8f;
  if (argc > 1) timerPeriod = atof(argv[1]);
  printf("usage: %s timerPeriod\n", argv[0]);
  App app;
  app.start(timerPeriod);
  getchar();
}
