#ifndef __LOG__
#define __LOG__

#include <thread>
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <chrono>
#include <vector>
#include <queue>
#include <unistd.h>
#include <fstream>
using namespace std;

#include "Framework/Queue.hpp"

template <int NUMBER_OF_QUEUES = 3, int QUEUE_SIZE = 100, int REPORT_LENGTH = 256, int WAIT_MICROSECONDS = 1000>
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

  Log(const char* fileName = "") {
    done = false;

    output.open(fileName);
    if (!output.is_open()) {
      output.copyfmt(cout);
      output.clear(cout.rdstate());
      output.basic_ios<char>::rdbuf(cout.rdbuf());
    }

    t = thread([&]() {
      logStartTime = chrono::high_resolution_clock::now();

      Report report;
      while (!done) {
        for (int i = 0; i < NUMBER_OF_QUEUES; ++i)
          while (queue[i].pop(report)) // XXX change to if to avoid starvation?
            priority.push(report);

        double now = chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now() - logStartTime).count();
        while ( (!priority.empty()) && ((now - priority.top().time) > 0.5)) {
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

  void operator()(int i, const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    // perror(buffer);
    va_end(args);

    Report report;
    report.time = chrono::duration_cast<chrono::duration<double>>(
        chrono::high_resolution_clock::now() - logStartTime).count();
    sprintf(report.text, "%.8lf|%s", report.time, buffer);

    queue[i].push(report);
  }
};

#endif
