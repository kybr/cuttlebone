#include "FileWatcher.hpp"
#include <iostream>
using namespace std;

struct Foo : FileWatcher {
  virtual void onModify(const char* filePath) {
    cout << filePath << " was changed" << endl;
  }
};

int main() {
  Foo foo;
  foo.watch("/tmp/foo");
  foo.start();
}
