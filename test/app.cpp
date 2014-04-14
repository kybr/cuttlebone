//#define LOG_FILE ("/tmp/log.txt")
#include "Framework/App.hpp"
#include <cstring>  // memset

// const char* name;
// const char* broadcast;
// bool simulation, audio, visual, device;
//ConfigurationData configurationData[] = {
//    {"00", "192.168.7.255", true, false, false, false},
//    {"01", "", false, false, true, false},
//    {"02", "", false, false, true, false},
//    {"03", "", false, false, true, false},
//    {"10", "", false, false, true, false},
//    {"11", "", false, false, true, false},
//    {"12", "", false, false, true, false},
//    {"13", "", false, false, true, false},
//    {"20", "", false, false, true, false},
//    {"21", "", false, false, true, false},
//    {"22", "", false, false, true, false},
//    {"23", "", false, false, true, false}, };

ConfigurationData configurationData[] = {
  {"image", "", false, false, true, false},
  {"quux.zzz", "192.168.2.255", true, false, false, false},
};

//#define N (10243)
#define N (300)

struct Vec3f {
  float x, y, z;

  Vec3f& operator+=(const Vec3f& right) {
    x += right.x;
    y += right.y;
    z += right.z;
    return *this;
  }
  Vec3f& operator*=(const float right) {
    x *= right;
    y *= right;
    z *= right;
    return *this;
  }
};

struct State {
  float r;
  Vec3f position[N];
};

struct MyApp : App<State> {
  MyApp() {
    App<State>::shouldLog = true;
    configuration = Configuration(configurationData);
    configuration.log();
    LOG("State is %d bytes", sizeof(State));
  }

  virtual void onSimulatorInit(State& state) {
    memset(&state, 0, sizeof(state));
  }

  virtual void onSimulate(float dt, State& state) {
    LOG("onSimulate(%f)", dt);
    Vec3f v{0.001f, 0.001f, 0.001f};
    for (int i = 0; i < N; ++i) {
      state.position[i] += v;
      state.position[i] *= 0.1f;
    }

    static float time = 0;
    time += dt / 10;
    state.r = time - (int)time;
  }

  virtual void onRendererLocal(float dt, State& state, int popCount) {
    LOG("onRendererLocal(%f)", dt);
  }
  virtual void onDraw(Graphics& g, const State& state) {}
  virtual void onSound(AudioIOData& io, const State& state) {}
};

int main() {
  MyApp app;
  app.start();
}
