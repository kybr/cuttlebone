#include <thread>
#include <iostream>
#include <list>
#include <cstdio>
#include <chrono>
#include <vector>
#include <queue>
#include <unistd.h>
#include <fstream>
using namespace std;

#include "Framework/Queue.hpp"
#include "Framework/Time.hpp"

chrono::high_resolution_clock::time_point logStartTime;

struct Message {
  double time;
  char text[256];
};

struct Log : Queue<Message, 20> {
  Message message;
  void operator()(const char* text) {
    message.time = duration_cast<duration<double>>(
        chrono::high_resolution_clock::now() - logStartTime).count();
    sprintf(message.text, "%.8lf|%s", message.time, text);
    push(message);
  }
};

struct LogManager {
  bool done = false;
  list<Log*> logs;
  thread t;
  ofstream output;

  LogManager(const char* fileName = "") { output.open(fileName); }

  Log& newLog() {
    Log* log = new Log();
    logs.push_front(log);
    return *log;
  }

  void start() {

    if (!output.is_open()) {
      output.copyfmt(cout);
      output.clear(cout.rdstate());
      output.basic_ios<char>::rdbuf(cout.rdbuf());
    }

    t = thread([&]() {
      struct CompareTime {
        bool operator()(Message const& a, Message const& b) {
          return a.time > b.time;
        }
      };
      priority_queue<Message, vector<Message>, CompareTime> priority;
      Message message;

      logStartTime = chrono::high_resolution_clock::now();

      while (!done) {
        for (auto log : logs)
          while (log->pop(message))
            priority.push(message);

        while (!priority.empty()) {
          output << priority.top().text << endl;
          priority.pop();
        }

        usleep(10000);
      }
    });
  };

  void stop() {
    done = true;
    t.join();
    output.close();
  }
};

int main() {
  LogManager logManager;
  logManager.start();

  thread a([&]() {
    Log& log(logManager.newLog());
    log("a|started");
    for (int i = 0; i < 10; i++) {
      usleep(16666);
      char m[10];
      sprintf(m, "a|%d", i);
      log(m);
    }
  });

  thread b([&]() {
    Log& log(logManager.newLog());
    log("b|started");
    for (int i = 0; i < 10; i++) {
      usleep(13026);
      char m[10];
      sprintf(m, "b|%d", i);
      log(m);
    }
  });

  thread c([&]() {
    Log& log(logManager.newLog());
    log("c|started");
    for (int i = 0; i < 10; i++) {
      usleep(11111);
      char m[10];
      sprintf(m, "c|%d", i);
      log(m);
    }
  });

  getchar();

  a.join();
  b.join();
  c.join();

  logManager.stop();
}
