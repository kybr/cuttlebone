#include "Cuttlebone/Cuttlebone.hpp"
using namespace cuttlebone;

#include <unistd.h>  // usleep

struct State {
  int data[100];
};

struct App : TakerApp<State> {
  virtual void started() { LOG("started()"); }
  virtual void got(State& state, double dt, int popCount) {
    if (! popCount) return;
    LOG("got() state.data[0] = %d popCount = %d", state.data[0], popCount);
  }
};

int main() { App().start(); }
