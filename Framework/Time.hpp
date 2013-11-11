#ifndef __MY_TIME__
#define __MY_TIME__

#include <chrono>
using namespace std;
using namespace std::chrono;

template <typename clock = high_resolution_clock>
struct Timestamp {
  time_point<clock> born;
  Timestamp() { born = clock::now(); }
  inline double stamp() {
    return (duration_cast<duration<double>>(clock::now() - born)).count();
  }
};

template <typename clock = high_resolution_clock>
struct Stopwatch {
  time_point<clock> start, end;
  void tic() { start = clock::now(); }
  inline double toc() {
    end = clock::now();
    duration<double> elapsed_seconds = end - start;
    return elapsed_seconds.count();
  }
};

#endif
