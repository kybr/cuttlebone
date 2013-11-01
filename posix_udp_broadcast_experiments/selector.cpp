#include "Stuff.hpp"
#include <iostream>
using namespace std;

#define N (512)

struct App : Selector<App> {
  unsigned char buffer[N];
  void onNewBuffer() {
    cout << "onNewBuffer(): " << (unsigned)buffer[0] << endl;
  }
};

int main() {
  App app;
  app.start(N);
  getchar();
  app.stop();
}
