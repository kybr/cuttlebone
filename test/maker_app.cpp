#include "Cuttlebone/Cuttlebone.hpp"
using namespace cuttlebone;

#include <unistd.h>  // usleep

struct State {
  int data[100];
};

struct App : MakerApp<State> {
  virtual void init(State& state) {
    state.data[0] = 1;
    LOG("init() state.data[0] = %d", state.data[0]);
  }
  virtual void update(State& state) {
    LOG("update() state.data[0] = %d", state.data[0]);
    state.data[0]++;
    usleep(200000);
  }
};

int main() { App().start(); }
