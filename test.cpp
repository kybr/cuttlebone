#include "allocore/io/al_AppWith.hpp"
using namespace al;

#define N (1000)
struct State {
  float data[N];
};

struct StateApp : AppWith<State> {
  StateApp() {
    init(); // for local apps use this init()
    //init("quux.zzz"); // for distributed apps use this init()
    initWindow();
  }

  virtual void onAnimate(double dt) {
    if (mode == WRITER) {
      // calculate here, filling data()
      //
      send();
    }
    else {
      poll();
      // decompress, up-sample, decorate, etc data() here
      //
    }
  }

  virtual void onDraw(Graphics& g, const Viewpoint&) {
    if ((mode == READER) || (mode == LOCAL)) {}
    else {
      // draw stuff
      //
    }
  }
};

int main() { StateApp().start(); }
