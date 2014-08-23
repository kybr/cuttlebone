//#define LOG_FILE ("/tmp/log.txt")
#include "Cuttlebone/Cuttlebone.hpp"

struct State {
  int data[100];
};

struct MyApp : Maker<State>, Taker<State> {
  MyApp() {
    Maker<State>::shouldLog = true;
    Taker<State>::shouldLog = true;
    LOG("MyApp() - State is %d bytes", sizeof(State));
  }
  virtual void firstRun() {
    LOG("firstRun()");
  }
  virtual void gotState(float dt, State& state, int popCount) {
    LOG("gotState() : state.data[0] = %d", state.data[0]);
  }
  virtual void setup(State& state) {
    LOG("setup()");
    state.data[0] = 0;
  }
  virtual void update(float dt, State& state) {
    state.data[0]++;
  }
  virtual void start() {
    thread a([this](){ Maker<State>::start(); });
    thread b([this](){ Taker<State>::start(); });
    getchar();
  }
};

int main() {
  LOG("main()");
  MyApp app;
  app.start();
}
