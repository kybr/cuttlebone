#include <thread>
#include <unistd.h>

using namespace std;

#define LOG_FILE_PATH ("")
#include "Framework/Log.hpp"
//#define log(...) do{}while(0)

int main(int argc, char* argv[]) {
  log("main|started");

  thread audio([&]() {
    log("audio|started");
    for (int i = 0; i < 345; i++) {
      usleep(2902);
      log("audio|%d", i);
    }
    log("audio|ended");
  });

  thread graphics([&]() {
    log("graphics|started");
    for (int i = 0; i < 60; i++) {
      usleep(16667);
      log("graphics|%d", i);
    }
    log("graphics|ended");
  });

  thread network([&]() {
    log("network|started");
    for (int i = 0; i < 10; i++) {
      usleep(100000);
      log("network|%d", i);
    }
    log("network|ended");
  });

  getchar();
  audio.join();
  graphics.join();
  network.join();
  log("main|ended");
}
