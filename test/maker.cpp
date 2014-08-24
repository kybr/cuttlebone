//#define LOG_FILE ("/tmp/log.txt")
#include "Cuttlebone/Cuttlebone.hpp"

using namespace cuttlebone;

#include <unistd.h>  // usleep

struct State {
  int data[100];
};

int main() {
  Maker<State> maker;
  maker.shouldLog = true;
  State* state = new State;
  state->data[0] = 1;
  maker.start();
  while (true) {
    maker.set(*state);
    state->data[0]++;
    usleep(500000);
  }
}
