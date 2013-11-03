#include "allocore/io/al_AppWith.hpp"
using namespace al;

#define N (1000)
struct State {
  float data[N];
};

extern   State& data();


struct StateApp : AppWith<StateApp,State> {
  StateApp() {
    init(); // for local apps use this init()
    //init("quux.zzz"); // for distributed apps use this init()
    initWindow();
  }

  onSimulate(double dt) {
    // Will be called or not depending on whether this machine is
    // the simulation creator
  }

  onLocalApproximationToSimulation(double dt) {
    // Called on non-simulator machines when onAnimate() would
    // have been called, but if no new State POD came since the last frame.
    // Allows, e.g., local interpolation, or carrying on with previous
    // frame's velocities and accelerations without computing full N-body
    // problem, etc.
    // Version 2 feature?
  }
  
  onAnimate() {
      // No longer exists in a State App?
    
      // Do we need to provide a method to "decompress" a state POD
      // "upsample"/"decorate"...?   Would be called once per POD.
  }
  
  
  virtual void onDraw(Graphics& g, const Viewpoint&) {
    // Will be called or not depending on whether this machine is
    // a video renderer?
    
    blah blah blah   data().agents[i]
    
    // Also need to allow the simulation machine to be on the frame
    // lock, which may require rendering a 5x5 pixel window of black 
    // or something.
  }
  
};

int main() { StateApp().start(); }


/*
 
 Advanced use cases possibly not needing to be supported:
 
 - Simulation step is smaller than rendering step (game physics)
 
 - Simulation step is bigger than rendering step (Dhilung)
 
*/
