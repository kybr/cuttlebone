#ifndef __PACKET__
#define __PACKET__

// template <typename T>
// struct UnionOf {
//   union {
//     T data;
//     char byte[sizeof(T)];
//   };
// };

#include <string.h>

template <unsigned SIZE>
struct Packet {
  struct Header {
    unsigned frameNumber, partNumber, partSize, totalPartCount;
  };
  enum {
    HEADER_SIZE = sizeof(Header),
    DATA_SIZE = SIZE - sizeof(Header),
  };
  Header header;
  char byte[DATA_SIZE];
};

template <typename STATE, typename PACKET>
struct PacketMaker {
  enum {
    TOTAL_PART_COUNT = 1 + sizeof(STATE) / PACKET::DATA_SIZE,
    LAST_DATA_SIZE = sizeof(STATE) % PACKET::DATA_SIZE
  };

  const STATE& state;
  unsigned frameNumber, partNumber;

  PacketMaker(const STATE& state, unsigned frameNumber)
      : state(state), frameNumber(frameNumber), partNumber(0) {}

  bool fill(PACKET& packet) {
    if (partNumber >= TOTAL_PART_COUNT) return false;

    packet.header.totalPartCount = TOTAL_PART_COUNT;
    packet.header.frameNumber = frameNumber;
    packet.header.partNumber = partNumber;
    if (partNumber == (TOTAL_PART_COUNT - 1))
      packet.header.partSize = LAST_DATA_SIZE;
    else
      packet.header.partSize = PACKET::DATA_SIZE;

    memcpy(packet.byte,
           (void*)(((unsigned long)&state) + (partNumber * PACKET::DATA_SIZE)),
           packet.header.partSize);

    partNumber++;
    return true;
  }
};

template <typename STATE, typename PACKET>
struct PacketTaker {
  enum {
    TOTAL_PART_COUNT = 1 + sizeof(STATE) / PACKET::DATA_SIZE,
    LAST_DATA_SIZE = sizeof(STATE) % PACKET::DATA_SIZE
  };

  STATE& state;
  unsigned part[TOTAL_PART_COUNT];

  PacketTaker(STATE& state) : state(state) {
    for (unsigned i = 0; i < TOTAL_PART_COUNT; ++i) part[i] = 0;
  }

  bool take(PACKET& packet) {
    bool foundAll = true;
    for (unsigned i = 0; i < TOTAL_PART_COUNT; ++i)
      if (part[i] != 1) foundAll = false;
    if (foundAll) return false;

    memcpy((void*)(((unsigned long)&state) +
                   (packet.header.partNumber * PACKET::DATA_SIZE)),
           packet.byte, packet.header.partSize);

    part[packet.header.partNumber] = 1;
    return true;
  }
};

#endif
