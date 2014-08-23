#ifndef INCLUDE_STATS_HPP
#define INCLUDE_STATS_HPP

namespace cuttlebone {

struct Stats {
  const char* header;
  int count;
  float period;
  Stats(const char* header = "FPS") : count(0), period(0), header(header) {}
  void operator()(float dt) {
    count++;
    period += dt;
    if (period > 1.0) {
      LOG("%s:%d", header, count);
      period -= 1.0;
      count = 0;
    }
  }
};

}  // cuttlebone

#endif

