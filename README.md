# Cuttlebone

This is a framework / toolkit for developing distributing multimedia applications. Use the BroadcastApp<State> to distribute application state information to all hosts on your network using udp broadcast. This system may be used with Cuttlefish or in the AlloSphere, using AlloSystem.

## dependencies

- c++11
- POSIX
- libssl-dev package on linux

## building (with AlloSystem)

Using the latest devel branch of AlloSystem, configure the cmake system so that it will find the Cuttlebone headers. Then, include `Cuttlebone/al_BroadcastOmniApp.hpp` in your code and follow the example of `example/use_broadcast_app.cpp`. 

## building (stand alone)

    mkdir build
    cd build
    cmake ..
    make
    ./app

