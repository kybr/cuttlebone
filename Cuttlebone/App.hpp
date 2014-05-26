#ifndef INCLUDE_APP_HPP
#define INCLUDE_APP_HPP

#include "Cuttlebone/Broadcaster.hpp"
#include "Cuttlebone/Configuration.hpp"
#include "Cuttlebone/Packet.hpp"
#include "Cuttlebone/Queue.hpp"
#include "Cuttlebone/Receiver.hpp"
#include "Cuttlebone/Selector.hpp"
#include "Cuttlebone/Time.hpp"
#include "Cuttlebone/Timer.hpp"
#include "Cuttlebone/Log.hpp"

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
  STATE* simulationState;
  Timestamp<> ts;
  float last, now;

  Queue<STATE> simulateBroadcast, receiveGraphicsRender, receiveAudioRender;
  // thread simulate, broadcast, receive;
  thread broadcast, receive, render;

  Configuration configuration;

  App()
      : shouldLog(false),
        done(false),
        waitingToStart(true),
        audioRenderState(new STATE),
        graphicsRenderState(new STATE),
        simulationState(new STATE),
        last(0),
        now(0) {}

  virtual ~App() {
    done = true;
    if (configuration.simulation) {
      // simulate.join();
      broadcast.join();
    }
    if (configuration.visual || configuration.audio) receive.join();
    if (configuration.visual) render.join();
  }

  virtual void start() {
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

    if (configuration.visual) {
      render = thread([&]() {
        while (waitingToStart) {
          usleep(LITTLE_WAIT_TIME_US);
        }

        while (!done) {
          int popCount = 0;
          while (receiveGraphicsRender.pop(*graphicsRenderState))
            popCount++;

          if (popCount) {
            static float localLast = 0, localNow = 0;
            localLast = localNow;
            localNow = ts.stamp();
            onRendererLocal(localNow - localLast, *graphicsRenderState,
                            popCount);
          }
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

    if (configuration.simulation) {
      onSimulatorInit(*simulationState);
      Timer::start(1 / 60.0f);
    }

    getchar();
  }

  // XXX
  void onTimer() {
    last = now;
    now = ts.stamp();
    onSimulate(now - last, *simulationState);
    simulateBroadcast.push(*simulationState);
  }

  virtual void onDraw(Graphics& g) { onDraw(g, *graphicsRenderState); }

  virtual void onSound(AudioIOData& io) {
    int popCount = 0;
    while (receiveAudioRender.pop(*audioRenderState)) popCount++;
    onSound(io, *audioRenderState);
  }
};
#endif
