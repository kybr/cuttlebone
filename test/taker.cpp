//#define LOG_FILE ("/tmp/log.txt")
#include "Cuttlebone/Cuttlebone.hpp"

using namespace cuttlebone;

struct State {
  int data[100];
};

int main() {
  Taker<State> taker;
  taker.shouldLog = true;
  State* state = new State;
  taker.start();
  while (true) {
    int popCount = taker.get(*state);
    if (popCount) LOG("got %d", state->data[0]);
  }
}
