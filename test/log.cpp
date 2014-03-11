#include <thread>
#include <unistd.h>

using namespace std;

#include "Framework/Log.hpp"

int main(int argc, char* argv[]) {
  Log<> log((argc == 2) ? argv[1] : "");

  thread audio([&]() {
    log(0, "audio|started");
    for (int i = 0; i < 10; i++) {
      usleep(16666);
      log(0, "audio|%d", i);
    }
    log(0, "audio|ended");
  });

  thread graphics([&]() {
    log(1, "graphics|started");
    for (int i = 0; i < 10; i++) {
      usleep(16666);
      log(1, "graphics|%d", i);
    }
    log(1, "graphics|ended");
  });

  thread network([&]() {
    log(2, "network|started");
    for (int i = 0; i < 10; i++) {
      usleep(16666);
      log(2, "network|%d", i);
    }
    log(2, "network|ended");
  });

  getchar();

  audio.join();
  graphics.join();
  network.join();
}
