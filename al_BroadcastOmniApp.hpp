#ifndef INCLUDE_AL_BROADCAST_APP_HPP
#define INCLUDE_AL_BROADCAST_APP_HPP

//#define LOG_FILE ("/tmp/file.txt") // log to a file instead of standard out
//#define DONT_COMPILE_LOG // like log never existed
#include "alloutil/Log.hpp"
#include "alloutil/HostRole.hpp"
#include "alloutil/Packet.hpp"
#include "alloutil/Queue.hpp"

#include "alloutil/al_OmniApp.hpp"
#include "allocore/sound/al_Ambisonics.hpp"
#include "allocore/sound/al_AudioScene.hpp"
#include "allocore/sound/al_Speaker.hpp"

#define LITTLE_WAIT_TIME_US (100)

namespace al {

Speaker speaker[] = {
  Speaker(1-1, 102.339087, 41.000000),
  Speaker(2-1, 65.479774, 41.000000),
  Speaker(3-1, 22.878659, 41.000000),
  Speaker(4-1, -22.878659, 41.000000),
  Speaker(5-1, -65.479774, 41.000000),
  Speaker(6-1, -102.339087, 41.000000),
  Speaker(7-1, -77.660913, 41.000000),
  Speaker(8-1, -114.520226, 41.000000),
  Speaker(9-1, -157.121341, 41.000000),
  Speaker(10-1, 157.121341, 41.000000),
  Speaker(11-1, 114.520226, 41.000000),
  Speaker(12-1, 77.660913, 41.000000),
  Speaker(17-1, 102.339087, 0.000000),
  Speaker(18-1, 89.778386, 0.000000),
  Speaker(19-1, 76.570355, 0.000000),
  Speaker(20-1, 62.630227, 0.000000),
  Speaker(21-1, 47.914411, 0.000000),
  Speaker(22-1, 32.455914, 0.000000),
  Speaker(23-1, 16.397606, 0.000000),
  Speaker(24-1, 0.000000, 0.000000),
  Speaker(25-1, -16.397606, 0.000000),
  Speaker(26-1, -32.455914, 0.000000),
  Speaker(27-1, -47.914411, 0.000000),
  Speaker(28-1, -62.630227, 0.000000),
  Speaker(29-1, -76.570355, 0.000000),
  Speaker(30-1, -89.778386, 0.000000),
  Speaker(31-1, -102.339087, 0.000000),
  Speaker(32-1, -77.660913, 0.000000),
  Speaker(33-1, -90.221614, 0.000000),
  Speaker(34-1, -103.429645, 0.000000),
  Speaker(35-1, -117.369773, 0.000000),
  Speaker(36-1, -132.085589, 0.000000),
  Speaker(37-1, -147.544086, 0.000000),
  Speaker(38-1, -163.602394, 0.000000),
  Speaker(39-1, -180.000000, 0.000000),
  Speaker(40-1, 163.602394, 0.000000),
  Speaker(41-1, 147.544086, 0.000000),
  Speaker(42-1, 132.085589, 0.000000),
  Speaker(43-1, 117.369773, 0.000000),
  Speaker(44-1, 103.429645, 0.000000),
  Speaker(45-1, 90.221614, 0.000000),
  Speaker(46-1, 77.660913, 0.000000),
  Speaker(49-1, 102.339087, -32.500000),
  Speaker(50-1, 65.479774, -32.500000),
  Speaker(51-1, 22.878659, -32.500000),
  Speaker(52-1, -22.878659, -32.500000),
  Speaker(53-1, -65.479774, -32.500000),
  Speaker(54-1, -102.339087, -32.500000),
  Speaker(55-1, -77.660913, -32.500000),
  Speaker(56-1, -114.520226, -32.500000),
  Speaker(57-1, -157.121341, -32.500000),
  Speaker(58-1, 157.121341, -32.500000),
  Speaker(59-1, 114.520226, -32.500000),
  Speaker(60-1, 77.660913, -32.500000),
};

Speaker localSpeaker[] = {Speaker(0, 45, 0), Speaker(1, -45, 0), };

template <typename STATE, unsigned PACKET_SIZE = 1400>
struct BroadcastApp : OmniApp {
  STATE simulationState, graphicsRenderState, audioRenderState;
  thread broadcast, receive;
  Queue<STATE> simulateBroadcast, receiveGraphicsRender, receiveAudioRender;
  HostRole hostRole;
  bool done, waitingToStart, greedy;

  Listener* sceneListener;
  AudioScene* scene;

  AudioScene& audioScene() { return *scene; }
  Listener& listener() { return *sceneListener; }

  virtual ~BroadcastApp() {
    done = true;
    if (hostRole.isSimulator) broadcast.join();
    if (hostRole.isGraphicsRenderer || hostRole.isAudioRenderer) receive.join();
  }

  BroadcastApp(unsigned port = 8888, bool greedy = true) : greedy(greedy) {
    log("BroadcastApp()");

    done = false;
    waitingToStart = true;

    hostRole.init();

    memset(&simulationState, 0, sizeof(STATE));
    memset(&graphicsRenderState, 0, sizeof(STATE));
    memset(&audioRenderState, 0, sizeof(STATE));

    if (hostRole.isSimulator) {
      log("creating broadcast thread");
      broadcast = thread([&]() {
        Broadcaster broadcaster;
        broadcaster.init(PACKET_SIZE, hostRole.broadcastIpAddress(), port);
        Packet<PACKET_SIZE> p;
        STATE state;
        int frame = 0;

        while (waitingToStart) {
          usleep(LITTLE_WAIT_TIME_US);
        }

        while (!done) {
          bool hadAny = false;
          if (greedy)
            while (simulateBroadcast.pop(state))
              hadAny = true;
          else if (simulateBroadcast.pop(state))
            hadAny = true;

          if (hadAny) {
            PacketMaker<STATE, Packet<PACKET_SIZE> > packetMaker(state, frame);
            while (packetMaker.fill(p))
              broadcaster.send((unsigned char*)&p);
            frame++;
          } else
            usleep(LITTLE_WAIT_TIME_US);
        }
      });
    }

    if (hostRole.isGraphicsRenderer || hostRole.isAudioRenderer) {
      log("creating receive thread");
      receive = thread([&]() {
        Receiver receiver;
        receiver.init(port);
        Packet<PACKET_SIZE> p;
        STATE state;

        while (waitingToStart) {
          usleep(LITTLE_WAIT_TIME_US);
        }

        while (!done) {

          if (!receiver.receive((unsigned char*)&p, PACKET_SIZE, 0.2f)) {
            usleep(LITTLE_WAIT_TIME_US);
            continue;
          }

        ABORT_FRAME:
          ;
          // wait until we're at the begining of a frame
          if (p.header.partNumber != 0)
            continue;

          PacketTaker<STATE, Packet<PACKET_SIZE> > packetTaker(
              state, p.header.frameNumber);

          packetTaker.take(p);

          while (!packetTaker.isComplete()) {
            if (receiver.receive((unsigned char*)&p, PACKET_SIZE, 0.2f)) {
              if (!packetTaker.take(p)) {
                log("ABORT FRAME. got a part from an unexpected frame before we finished this frame");
                packetTaker.summary();
                goto ABORT_FRAME;
              }
            } else
              usleep(LITTLE_WAIT_TIME_US);
          }

          if (hostRole.isGraphicsRenderer)
            receiveGraphicsRender.push(state);

          if (hostRole.isAudioRenderer)
            receiveAudioRender.push(state);
        }
      });
    }

    if (hostRole.isSimulator || hostRole.isGraphicsRenderer) initWindow();

    if (hostRole.isAudioRenderer && hostRole.isSimulator &&
        hostRole.isGraphicsRenderer) {
      // This is a laptop; init with default 2 channels
      scene = new AudioScene(2, 1, 512);
      sceneListener = scene->createListener(2);
      sceneListener->numSpeakers(2);
      for (int i = 0; i < 2; ++i)
        sceneListener->speakerPos(i, localSpeaker[i].deviceChannel,
                             localSpeaker[i].azimuth,
                             localSpeaker[i].elevation);
      initAudio();
    } else if (hostRole.isAudioRenderer) {
      // This is the AlloSphere audio machine; open 60 channels
      scene = new AudioScene(3, 3, 512);
      sceneListener = scene->createListener(54);
      sceneListener->numSpeakers(54);
      for (int i = 0; i < 54; ++i)
        sceneListener->speakerPos(i, speaker[i].deviceChannel, speaker[i].azimuth,
                             speaker[i].elevation);
      initAudio("ECHOX5", 44100, 512, 60, 60);
    }
    else {
      scene = new AudioScene(2, 1, 512);
      sceneListener = scene->createListener(2);
      sceneListener->numSpeakers(2);
      for (int i = 0; i < 2; ++i)
        sceneListener->speakerPos(i, localSpeaker[i].deviceChannel,
                             localSpeaker[i].azimuth,
                             localSpeaker[i].elevation);
    }

    // need to wait until the audio and graphics threads are ready before we
    // start the broadcast and receive threads.
    //
    usleep(100000);
    waitingToStart = false;
    log("end of BroadcastApp constructor...");
  }

  virtual void onSimulatorInit(STATE& state) = 0;
  virtual void onSimulate(double dt, STATE& state) = 0;
  virtual void onRendererLocal(double dt, STATE& state,
                               bool gotMostRecentState) = 0;
  virtual void onDraw(Graphics& g, const STATE& state) = 0;
  virtual void onSound(AudioIOData& io, const STATE& state) = 0;

  virtual void onAnimate(double dt) {
    static bool firstTime = true;

    if (hostRole.isSimulator) {
      if (firstTime) {
        firstTime = false;
        onSimulatorInit(simulationState);
      } else
        onSimulate(dt, simulationState);
      simulateBroadcast.push(simulationState);
    }

    if (hostRole.isGraphicsRenderer) {
      bool hadAny = false;
      if (greedy)
        while (receiveGraphicsRender.pop(graphicsRenderState)) {
          hadAny = true;
        }
      else if (receiveGraphicsRender.pop(graphicsRenderState)) {
        hadAny = true;
      }

      if (!hadAny) {
        log("RENDER QUEUE EMPTY.. using old frame ");
      }

      onRendererLocal(dt, graphicsRenderState, hadAny);
    }
  }

  virtual void onDraw(Graphics& g) { onDraw(g, graphicsRenderState); }

  virtual void onSound(AudioIOData& io) {
    bool hadAny = false;
    if (receiveAudioRender.pop(audioRenderState)) {
      hadAny = true;
    }
    onSound(io, audioRenderState);
  }
};

}  // namespace al
#endif
