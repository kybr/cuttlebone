#ifndef __LOG__
#define __LOG__

// c++
#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>
// c
#include <cassert>
#include <cstdio>
#include <cstdarg>
// system
#include <unistd.h>
using namespace std;

#include "Framework/Queue.hpp"

#ifndef LOG_FILE_PATH
#define LOG_FILE_PATH ("")
#endif

#define log(...)                                                       \
  do {                                                                 \
    Log<>::instance().report(std::this_thread::get_id(), __VA_ARGS__); \
  } while (0)

template <int NUMBER_OF_QUEUES = 7, int QUEUE_SIZE = 100,
          int REPORT_LENGTH = 256, int WAIT_MICROSECONDS = 1000>
struct Log {

  struct Report {
    double time;
    char text[REPORT_LENGTH];
  };

  struct CompareTime {
    bool operator()(Report const& a, Report const& b) {
      return a.time > b.time;
    }
  };

  Queue<Report, QUEUE_SIZE> queue[NUMBER_OF_QUEUES];
  bool done;
  thread t;
  ofstream output;
  priority_queue<Report, vector<Report>, CompareTime> priority;
  chrono::high_resolution_clock::time_point logStartTime;
  std::unordered_map<std::thread::id, int> queueThreadMap;
  int n;

  static Log& instance() {
    static Log instance;
    return instance;
  }

  Log() {
    n = 0;
    done = false;

    output.open(LOG_FILE_PATH);
    if (!output.is_open()) {
      output.copyfmt(cout);
      output.clear(cout.rdstate());
      output.basic_ios<char>::rdbuf(cout.rdbuf());
    }

    logStartTime = chrono::high_resolution_clock::now();
    t = thread([&]() {

      Report report;
      while (!done) {
        for (int i = 0; i < NUMBER_OF_QUEUES; ++i)
          while (queue[i].pop(report))  // XXX change to if to avoid starvation?
            priority.push(report);

        double now = chrono::duration_cast<chrono::duration<double>>(
            chrono::high_resolution_clock::now() - logStartTime).count();
        while ((!priority.empty()) && ((now - priority.top().time) > 0.5)) {
          output << priority.top().text << endl;
          priority.pop();
        }

        usleep(WAIT_MICROSECONDS);
      }

      for (int i = 0; i < NUMBER_OF_QUEUES; ++i)
        while (queue[i].pop(report))
          priority.push(report);

      while (!priority.empty()) {
        output << priority.top().text << endl;
        priority.pop();
      }
    });
  }

  ~Log() {
    done = true;
    t.join();
    output.close();
  }

  void report(std::thread::id tid, const char* format, ...) {
    Report report;
    report.time = chrono::duration_cast<chrono::duration<double>>(
        chrono::high_resolution_clock::now() - logStartTime).count();

    if (queueThreadMap.find(tid) == queueThreadMap.end()) {
      // XXX this is not thread-safe, IMHO.
      queueThreadMap[tid] = n;
      n++;
      assert(n < NUMBER_OF_QUEUES);
    }

    int q = queueThreadMap[tid];

    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    // perror(buffer);
    va_end(args);

    sprintf(report.text, "%.8lf|%d|%s", report.time, q, buffer);
    queue[q].push(report);
  }
};

#endif
