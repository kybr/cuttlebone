#include "Framework/Timer.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;

struct App : Timer {
  unsigned char n;
  App() {
    n = 0;
  }
  virtual void onTimer() {
    cout << (unsigned)n << endl;
    n++;
  }
};

int main() {
  App app;
  app.start(1 / 60.0f);
  getchar();
}
