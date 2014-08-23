# Cuttlebone

This is a framework / toolkit for developing distributing multimedia
applications. Use the Maker<State> and Taker<State> abstractions to distribute
and collect application state information between apps on your wired network
using udp broadcast. This system may be used with Cuttlefish or in the
AlloSphere, using AlloSystem.

## Dependencies

- c++11
- POSIX

## Compile examples and tests

    mkdir build
    cd build
    cmake ..
    make -j3

### on the pi

    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/pi.osx ..
    make -j3

### Using with AlloSystem

use flags.txt to compile using c++11 (and libcxx on osx). use al::Simulator,
al::AudioRenderer, and al::GraphicsRenderer.
