#include <iostream>
#include <string>
#include <sys/types.h>
using namespace std;

namespace alky {

class Log {
 public:
  void log(string s) {
    // enqueue into a a thread-safe queue
    cout << s << endl;
  }

  // this stuff makes this class a "singleton"
  //
  static Log& instance() {
    static Log instance;
    return instance;
  }

 private:
  Log() {};
  Log(Log const&);
  void operator=(Log const&);
};

void log(string s) { Log::instance().log(s); }

};  // namespace alky

int main() {
  using namespace alky;
  log("this is a message");
}
