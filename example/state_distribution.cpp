#include "Framework/Packet.hpp"
#include "Framework/Receiver.hpp"
#include "Framework/Broadcaster.hpp"
#include "Framework/Queue.hpp"

#include <unistd.h>  // usleep
#include <iostream>  // cout
#include <thread>
#include <string.h>
using namespace std;

#define PACKET_SIZE (1400)

//#include <cstddef>
//#include <string>
//#include <stdlib.h>

// must be a POD type
//
struct State {
  unsigned n;
  char data[200000];
  void zero() { memset(this, 0, sizeof(State)); }
};

int main(int argc, char* argv[]) {
  thread simulate, broadcast, receive, render;

  Queue<State> simulateBroadcast, receiveRender;

  unsigned simulateBroadcastWasFull = 0;
  unsigned simulateBroadcastWasEmpty = 0;
  unsigned receiveRenderWasFull = 0;
  unsigned receiveRenderWasEmpty = 0;

  bool waitingToStart = true, done = false;

  simulate = thread([&]() {
    State s;
    s.zero();
    while (waitingToStart)
      usleep(1000);

    while (!done) {
      sprintf(s.data, "%u", s.n);
      if (simulateBroadcast.push(s)) {
        // printf("push simulated %s\n", s.data);
        s.n++;
      } else
        simulateBroadcastWasFull++;
      usleep(16666);
    }
  });

  broadcast = thread([&]() {
    Broadcaster broadcaster;
    broadcaster.init(PACKET_SIZE, "127.0.0.1", 8888);
    Packet<PACKET_SIZE> p;
    State s;
    int frame = 0;
    while (waitingToStart)
      usleep(1000);

    while (!done) {
      if (simulateBroadcast.pop(s)) {  // XXX while() for greed

        PacketMaker<State, Packet<PACKET_SIZE> > packetMaker(s, frame);
        while (packetMaker.fill(p))
          broadcaster.send((unsigned char*)&p);
        frame++;

      } else {
        simulateBroadcastWasEmpty++;
        usleep(1000);
      }
    }
  });

  receive = thread([&]() {
    Receiver receiver;
    receiver.init(8888);
    Packet<PACKET_SIZE> p;
    State s;
    while (waitingToStart)
      usleep(1000);

    while (!done) {

      if (!receiver.receive((unsigned char*)&p, PACKET_SIZE, 0.2f)) {
        usleep(1000);
        continue;
      }

      // wait until we're at the begining of a frame
      if (p.header.partNumber != 0)
        continue;

      PacketTaker<State, Packet<PACKET_SIZE> > packetTaker(
          s, p.header.frameNumber);

      packetTaker.take(p);

      while (!packetTaker.isComplete()) {
        if (receiver.receive((unsigned char*)&p, PACKET_SIZE, 0.2f)) {
          /*
          printf("received part %u of frame %u\n", p.header.partNumber,
                 p.header.frameNumber);
          */
          if (!packetTaker.take(p)) {
            // got a part from an unexpected frame before we finished this frame
            printf("lost frame\n");
            goto ABORT_FRAME;
          }
        } else {
          usleep(1000);
        }
      }

      // we're all done, try to push
      if (!receiveRender.push(s))
        receiveRenderWasFull++;
    ABORT_FRAME:
      ;
    }
  });

  render = thread([&]() {
    State s;
    while (waitingToStart)
      usleep(1000);

    while (!done) {
      if (receiveRender.pop(s)) {
        // make a pretty picture with s
        printf("render %s\n", s.data);
      } else
        receiveRenderWasEmpty++;
      usleep(16666);
    }
  });

  waitingToStart = false;
  getchar();
  done = true;
  simulate.join();
  broadcast.join();
  receive.join();
  render.join();

  cout << "simulateBroadcastWasFull:" << simulateBroadcastWasFull << endl;
  cout << "simulateBroadcastWasEmpty:" << simulateBroadcastWasEmpty << endl;
  cout << "receiveRenderWasFull:" << receiveRenderWasFull << endl;
  cout << "receiveRenderWasEmpty:" << receiveRenderWasEmpty << endl;
}
