// now.cpp
//
// this test is supposed to measure the practical, worst-case wait time for
// calls to now(). however, this ends up measuring worst-case thread
// interruption time.
//
//

#include <thread>
#include <chrono>
#include <unistd.h>

using namespace std;

//#define LOG_FILE ("/tmp/file.txt") // LOG to a file instead of standard out
//#define DONT_COMPILE_LOG // like LOG never existed
#include "Framework/Log.hpp"

#define N (3)

int main(int argc, char* argv[]) {
  using namespace std::chrono;
  double result[N];
  thread t[N];
  bool done = false;
  bool wait = true;

  auto test = [&result, &wait, &done](int i) {
    LOG("thread %d waiting...", i);
    double max = 0.0;
    while (wait)
      ;
    while (!done) {
      high_resolution_clock::time_point then = high_resolution_clock::now();
      high_resolution_clock::time_point now = high_resolution_clock::now();
      double delta = duration_cast<duration<double> >(now - then).count();
      if (delta > max) {
        max = delta;
        LOG("thread %d :: %lf", max);
      }
    }
    result[i] = max;
  };

  for (int i = 0; i < N; i++) t[i] = thread(test, i);
  wait = false;
  getchar();
  done = true;
  for (int i = 0; i < N; i++) t[i].join();
  for (int i = 0; i < N; i++) cout << result[i] << endl;
}
