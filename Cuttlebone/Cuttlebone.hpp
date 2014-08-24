#ifndef INCLUDE_APP_HPP
#define INCLUDE_APP_HPP

#include "Cuttlebone/Broadcaster.hpp"
#include "Cuttlebone/Stats.hpp"
#include "Cuttlebone/Log.hpp"
#include "Cuttlebone/Packet.hpp"
#include "Cuttlebone/Queue.hpp"
#include "Cuttlebone/Receiver.hpp"
#include "Cuttlebone/Time.hpp"
#include "Cuttlebone/Timer.hpp"

#include <thread>
using namespace std;

namespace cuttlebone {

// TODO
// - Simple Maker/Taker with no Timers or loops, just start/stop
// - MakerApp/TakerApp using AppLoop abstraction
// - Factor out lambdas for broadcasting and receiving
// - Use Timer to make a "Throttler" class for limiting throughput/CPU
//

template <typename STATE, unsigned PACKET_SIZE = 1400, unsigned PORT = 63059>
struct Maker {

  const char* broadcastIp;
  bool shouldLog;
  bool done;
  bool waitingToStart;
  thread broadcast;
  Queue<STATE> queue;
  STATE* state;

  virtual void set() { queue.push(*state); }

  Maker(const char* broadcastIp = "127.0.0.1")
      : broadcastIp(broadcastIp),
        shouldLog(false),
        done(false),
        waitingToStart(true),
        state(new STATE) {}

  virtual void start() {
    broadcast = thread([&]() {
      Broadcaster broadcaster;
      broadcaster.init(PACKET_SIZE, broadcastIp, PORT, false);
      Packet<PACKET_SIZE> p;
      STATE* state = new STATE;
      int frame = 0;

      while (!done) {
        int popCount = 0;
        while (queue.pop(*state))
          popCount++;

        if (popCount) {
          if (shouldLog)
            LOG("sent packet %d", frame);
          PacketMaker<STATE, Packet<PACKET_SIZE> > packetMaker(*state, frame);
          while (packetMaker.fill(p))
            broadcaster.send((unsigned char*)&p);
          frame++;
        }
      }

      delete state;
    });
  }

  virtual void stop() {
    done = true;
    broadcast.join();
  }
};

template <typename STATE, unsigned PACKET_SIZE = 1400, unsigned PORT = 63059>
struct Taker {

  bool shouldLog;
  bool done;
  bool waitingToStart;
  thread receive;
  Queue<STATE> queue;
  STATE* state;

  virtual int get() {
    int popCount = 0;
    while (queue.pop(*state)) popCount++;
    return popCount;
  }

  Taker()
      : shouldLog(false), done(false), waitingToStart(true), state(new STATE) {}

  virtual void start() {
    receive = thread([&]() {
      Receiver receiver;
      receiver.init(PORT, false);
      Packet<PACKET_SIZE> p;
      STATE* localState = new STATE;

      while (waitingToStart)
        usleep(LITTLE_WAIT_TIME_US);

      while (!done) {

        if (!receiver.receive((unsigned char*)&p, PACKET_SIZE, 0.2f))
          continue;

      ABORT_FRAME:
        ;
        // wait until we're at the begining of a frame
        if (p.header.partNumber != 0)
          continue;

        PacketTaker<STATE, Packet<PACKET_SIZE> > packetTaker(
            *localState, p.header.frameNumber);

        packetTaker.take(p);

        while (!packetTaker.isComplete()) {
          if (receiver.receive((unsigned char*)&p, PACKET_SIZE, 0.2f)) {
            if (!packetTaker.take(p)) {
              // got a part from an unexpected frame before we finished this
              // frame
              LOG("ABORT FRAME");
              packetTaker.summary();
              goto ABORT_FRAME;
            }
          }
        }

        if (shouldLog)
          LOG("got packet %d", p.header.frameNumber);

        queue.push(*state);
      }

      delete localState;
    });

    waitingToStart = false;
  }

  virtual void stop() {
    done = true;
    receive.join();
  }
};

template <typename STATE, unsigned PACKET_SIZE = 1400, unsigned PORT = 63059>
struct MakerApp : AppLoop {
  virtual void init(STATE& state) = 0;
  virtual void update(STATE& state) = 0;

  virtual void setup() {
    // XXX ordering?
    init(*state);
    MakerApp::start();
  }

  virtual void loop() {
    update(*state);
    MakerApp::set();
  }

  virtual void cleanup() { MakerApp::stop(); }
};

template <typename STATE, unsigned PACKET_SIZE = 1400, unsigned PORT = 63059>
struct TakerApp : AppLoop {
  virtual void started() = 0;
  virtual void got(STATE& state, double dt, int popCount) = 0;

  virtual void setup() {
    // XXX ordering?
    started();
    TakerApp::start();
  }

  virtual void loop() {
    int popCount = TakerApp::get();
    got(*state, 1.0, popCount);
  }

  virtual void cleanup() { TakerApp::stop(); }
};

}  // cuttlebone

#endif
