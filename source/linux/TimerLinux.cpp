#include "Cuttlebone/Timer.hpp"
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <assert.h>
#include <pthread.h>

namespace cuttlebone {

Timer* that;
static void func(int);

struct TimerImpl {
  double period;
  timer_t timerid;
  struct itimerspec timer_settings;
  struct sigevent sevp;

  TimerImpl(Timer* t) : period(1.0 / 60) { that = t; }

  void rate(double period) {}

  void start() {
    sevp.sigev_notify = SIGEV_SIGNAL;
    sevp.sigev_signo = SIGUSR1;
    sevp.sigev_value.sival_ptr = (void*)this;
    sevp.sigev_notify_attributes = 0;  // important
    signal(SIGUSR1, func);
    assert(timer_create(CLOCK_REALTIME, &sevp, &timerid) == 0);

    int seconds = (int)period;
    int nanoseconds = (period - (int)period) * 1000000000;
    if (nanoseconds > 999999999) nanoseconds = 999999999;

    timer_settings.it_interval.tv_sec = seconds;
    timer_settings.it_interval.tv_nsec = nanoseconds;
    timer_settings.it_value.tv_sec = 0;
    timer_settings.it_value.tv_nsec = 1;
    assert(timer_settime(timerid, 0, &timer_settings, 0) == 0);
  }

  void stop() {
    timer_settings.it_interval.tv_sec = 0;
    timer_settings.it_interval.tv_nsec = 0;
    timer_settings.it_value.tv_sec = 0;
    timer_settings.it_value.tv_nsec = 0;
    assert(timer_settime(timerid, 0, &timer_settings, 0) == 0);
  }
};

Timer::Timer() : impl(new TimerImpl(this)) {}
Timer::~Timer() { delete impl; }
void Timer::start() { impl->start(); }
void Timer::stop() { impl->stop(); }
void Timer::rate(double period) { impl->period = period; }
static void func(int) { that->onTimer(); }

}  // cuttlebone
