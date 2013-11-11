#ifndef __TIMER__
#define __TIMER__

#include <stdlib.h>

#ifdef _WIN32
#elif __linux__
#include <time.h>
#include <signal.h>
#include <assert.h>
#include <pthread.h>
#elif __APPLE__
#include <dispatch/dispatch.h>
#else
#warn "this system is not known"
#endif

struct Timer;
Timer* that;
#ifdef _WIN32
#elif __linux__
void function(int);
#elif __APPLE__
void function(void*);
#endif

struct Timer {
  virtual void onTimer() = 0;

#ifdef _WIN32
#elif __linux__

  timer_t timerid;
  struct itimerspec timer_settings;
  struct sigevent sevp;

  void start(float period) {
    that = this;
    sevp.sigev_notify = SIGEV_SIGNAL;
    sevp.sigev_signo = SIGUSR1;
    sevp.sigev_value.sival_ptr = (void*)this;
    sevp.sigev_notify_attributes = 0;  // important
    signal(SIGUSR1, function);
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

#elif __APPLE__

  dispatch_source_t timer;

  void start(float period) {
    that = this;
    uint64_t interval = period * 1000000000;
    uint64_t leyway = 1000ULL;
    dispatch_queue_t queue =
        dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0,
                                   DISPATCH_TIMER_STRICT, queue);
    if (!timer) exit(-1);
    dispatch_source_set_timer(timer, DISPATCH_TIME_NOW, interval, leyway);
    dispatch_source_set_event_handler_f(timer, function);
    dispatch_resume(timer);
  }

  void stop() { dispatch_source_cancel(timer); }

#endif  // __APPLE__
};

#ifdef _WIN32
#elif __linux__
void function(int) { that->onTimer(); }
#elif __APPLE__
void function(void*) { that->onTimer(); }
#endif
#endif  // __TIMER__
