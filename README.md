# Cuttlebone

This is a framework / toolkit for developing distributing multimedia applications. Use the BroadcastApp<State> to distribute application state information to all hosts on your network using udp broadcast. This system may be used with Cuttlefish or in the AlloSphere, using AlloSystem.

## dependencies

- c++11
- POSIX

## building (with AlloSystem)

see alloSystemExample/

Since Cuttlebone uses c++11, you must use a compiler that supports c++11. I use Xcode 5.1 on OSX 10.9. I know that GCC 4.8 on Ubuntu 13 also works. To set the -std=c++11 flag with an AlloSystem AlloProject, add this to the top of your CMakeLists.txt.

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11)

If you're on Mountain Lion (OSX 10.8), then you might have to add the -stdlib=libc++ flag too, like this:

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -stdlib=libc++)


## building (stand alone)

    mkdir build
    cd build
    cmake ..
    make

### on the pi

    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/ky.pi.osx ..
    make
