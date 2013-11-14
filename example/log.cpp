#include "Framework/Time.hpp"
#include "Framework/Queue.hpp"

#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;

struct String {
  char buffer[100];
};

int main() {
  bool complete = false;

  Queue<String, 100> a_q, b_q;

  thread a([&]() {
    Timestamp<> t;
    int n = 0;
    while (!complete) {
      String s;
      sprintf(s.buffer, "%f: %d message from a thread.", t.stamp(), n);
      a_q.push(s);
      n++;
      usleep(1e6l);
    }
  });

  thread b([&]() {
    int n = 0;
    while (!complete) {
      String s;
      sprintf(s.buffer, "%d message from b thread.", n);
      b_q.push(s);
      n++;
      usleep(1e6l);
    }
  });

  thread p([&]() {
    String s;
    while (!complete) {
      if (a_q.pop(s))
        cout << s.buffer << endl;
      if (b_q.pop(s))
        cout << s.buffer << endl;
      usleep(1e6l);
    }
  });

  getchar();
  complete = true;
  a.join();
  b.join();
  p.join();
}
