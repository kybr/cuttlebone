//#define LOG_FILE ("/tmp/log.txt")
#include "Cuttlebone/Help.hpp"

struct State {
  int data[100];
};

struct MyApp : Pusher<State> {
  MyApp() {
    shouldLog = true;
    LOG("MyApp() - State is %d bytes", sizeof(State));
  }
  virtual void setup(State& state) {
    LOG("setup()");
    state.data[0] = 0;
  }
  virtual void update(float dt, State& state) {
    state.data[0]++;
  }
};

int main() {
  LOG("main()");
  MyApp app;
  app.start();
}
