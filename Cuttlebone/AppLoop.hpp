#ifndef CUTTLEBONE_APPLOOP
#define CUTTLEBONE_APPLOOP
namespace cuttlebone {
#include <iostream>

struct AppLoop {
  virtual void setup() = 0;
  virtual void loop() = 0;
  virtual void cleanup() = 0;

  AppLoop() {}
  virtual ~AppLoop() {}

  bool running = false;

  void start() {
    // XXX - make an error or warning if this is not the main thread
    setup();
    running = true;
    while (running) loop();
    cleanup();
  }

  void stop(const char* message = "") {
    std::cout << message << std::endl;
    running = false;
  }

  void die(const char* message = "") {
    std::cerr << message << std::endl;
    running = false;
  }
};

}  // cuttlebone
#endif
