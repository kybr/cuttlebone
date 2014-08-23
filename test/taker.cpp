//#define LOG_FILE ("/tmp/log.txt")
#include "Cuttlebone/Cuttlebone.hpp"

struct State {
  int data[100];
};

struct MyApp : Taker<State> {
  MyApp() {
    shouldLog = true;
    LOG("MyApp() - State is %d bytes", sizeof(State));
  }
  virtual void firstRun() { LOG("firstRun()"); }
  virtual void gotState(float dt, State& state, int popCount) {
    LOG("gotState() : state.data[0] = %d", state.data[0]);
  }
};

int main() {
  LOG("main()");
  MyApp app;
  app.start();
}
