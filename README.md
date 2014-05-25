# Cuttlebone

This is a framework / toolkit for developing distributing multimedia applications. Use the BroadcastApp<State> to distribute application state information to all hosts on your network using udp broadcast. This system may be used with Cuttlefish or in the AlloSphere, using AlloSystem.

## dependencies

- c++11
- POSIX
- libssl-dev package on linux

## building (with AlloSystem)

Using the latest devel branch of AlloSystem, configure the cmake system so that it will find the Cuttlebone headers. Then, include `Cuttlebone/al_BroadcastOmniApp.hpp` in your code and follow the example of `use_broadcast_app.cpp`. 

Since Cuttlebone uses c++11, you must use a compiler that supports c++11. I use Xcode 5.1 on OSX 10.9. I know that GCC 4.8 on Ubuntu 13 also works. To set the -std=c++11 flag with an AlloSystem AlloProject, add this to the top of your CMakeLists.txt.

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

If you're on Mountain Lion (OSX 10.8), then you might have to add the -stdlib=libc++ flag too, like this:

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -stdlib=libc++ -Wno-deprecated-declarations")



## building (stand alone)

    mkdir build
    cd build
    cmake ..
    make
    ./app

