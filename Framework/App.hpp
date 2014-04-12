#ifndef INCLUDE_APP_HPP
#define INCLUDE_APP_HPP

#include "Framework/Broadcaster.hpp"
#include "Framework/Checksum.hpp"
#include "Framework/Configuration.hpp"
#include "Framework/Packet.hpp"
#include "Framework/Queue.hpp"
#include "Framework/Receiver.hpp"
#include "Framework/Selector.hpp"
#include "Framework/Time.hpp"
#include "Framework/Timer.hpp"
#include "Framework/Log.hpp"

#include <thread>
using namespace std;

#define LITTLE_WAIT_TIME_US (100)

#define PORT (8888)

struct Graphics {
  int dummy;
};

struct AudioIOData {
  int dummy;
};

template <typename STATE, unsigned PACKET_SIZE = 1400>
struct App : Timer {

  virtual void onSimulatorInit(STATE& state) = 0;
  virtual void onSimulate(float dt, STATE& state) = 0;
  virtual void onRendererLocal(float dt, STATE& state, int popCount) = 0;
  virtual void onDraw(Graphics& g, const STATE& state) = 0;
  virtual void onSound(AudioIOData& io, const STATE& state) = 0;

  bool shouldLog;
  bool done;
  bool waitingToStart;
  STATE* audioRenderState;
  STATE* graphicsRenderState;

  Queue<STATE> simulateBroadcast, receiveGraphicsRender, receiveAudioRender;
  thread simulate, broadcast, receive;

  Configuration configuration;

  App()
      : shouldLog(false),
        done(false),
        waitingToStart(true),
        audioRenderState(new STATE),
        graphicsRenderState(new STATE) {}

  virtual ~App() {
    done = true;
    if (configuration.simulation) {
      simulate.join();
      broadcast.join();
    }
    if (configuration.visual || configuration.audio) receive.join();
  }

  void start() {
    if (configuration.simulation) {
      broadcast = thread([&]() {
        Broadcaster broadcaster;
        broadcaster.init(PACKET_SIZE, configuration.broadcast, PORT, false);
        Packet<PACKET_SIZE> p;
        STATE* state = new STATE;
        int frame = 0;

        while (waitingToStart) {
          usleep(LITTLE_WAIT_TIME_US);
        }

        while (!done) {
          int popCount = 0;
          while (simulateBroadcast.pop(*state))
            popCount++;

          if (popCount) {
            if (shouldLog)
              LOG("sending packet %d", frame);
            PacketMaker<STATE, Packet<PACKET_SIZE> > packetMaker(*state, frame);
            while (packetMaker.fill(p))
              broadcaster.send((unsigned char*)&p);
            frame++;
          }
        }
      });

      simulate = thread([&]() {
        Timestamp<> ts;
        double last = 0, now = 0;
        STATE* state = new STATE;

        while (waitingToStart) {
          usleep(LITTLE_WAIT_TIME_US);
        }

        onSimulatorInit(*state);
        now = ts.stamp();
        while (!done) {
          last = now;
          now = ts.stamp();
          onSimulate(now - last, *state);
          simulateBroadcast.push(*state);
        }
      });
    }

    if (configuration.visual || configuration.audio) {
      receive = thread([&]() {
        Receiver receiver;
        receiver.init(PORT, false);
        Packet<PACKET_SIZE> p;
        STATE* state = new STATE;

        while (waitingToStart) {
          usleep(LITTLE_WAIT_TIME_US);
        }

        while (!done) {

          if (!receiver.receive((unsigned char*)&p, PACKET_SIZE, 0.2f)) {
            continue;
          }

        ABORT_FRAME:
          ;
          // wait until we're at the begining of a frame
          if (p.header.partNumber != 0)
            continue;

          PacketTaker<STATE, Packet<PACKET_SIZE> > packetTaker(
              *state, p.header.frameNumber);

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

          if (configuration.visual)
            receiveGraphicsRender.push(*state);

          if (configuration.audio)
            receiveAudioRender.push(*state);
        }
      });
    }

    // if (configuration.visual)
    //  ;  // initWindow(Window::Dim(800, 600), "", 60.0);

    // if (configuration.audio && configuration.simulation &&
    //    configuration.visual) {
    //} else if (configuration.audio) {
    //} else {
    //}

    waitingToStart = false;

    getchar();
  }

  virtual void onAnimate(double dt) {
    if (configuration.visual) {
      int popCount = 0;
      while (receiveGraphicsRender.pop(*graphicsRenderState)) popCount++;
      onRendererLocal(dt, *graphicsRenderState, popCount);
    }
  }

  void onTimer() {}

  virtual void onDraw(Graphics& g) { onDraw(g, *graphicsRenderState); }

  virtual void onSound(AudioIOData& io) {
    int popCount = 0;
    while (receiveAudioRender.pop(*audioRenderState)) popCount++;
    onSound(io, *audioRenderState);
  }
};
#endif
