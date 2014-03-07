#include <thread>
#include <iostream>
#include <sys/types.h>
using namespace std;
int main() {
  thread a([]() { cout << hash<thread::id>()(this_thread::get_id()) << endl; });
  a.join();
  cout << hash<thread::id>()(this_thread::get_id()) << endl;
  //  getchar();
}
