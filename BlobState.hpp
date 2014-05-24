#ifndef __BLOB_STATE__
#define __BLOB_STATE__

#include <vector>
#include <string>
#include <sstream>
using namespace std;

//#define ICOSPHERE_FILE "/tmp/icosphere_3.txt"
//#define N (642)

#define ICOSPHERE_FILE "/tmp/icosphere_4.txt"
#define N (2562)

//#define ICOSPHERE_FILE "/tmp/icosphere_5.txt"
//#define N (10242)

struct Vertex {
  float x, y, z;
  Vertex& operator=(const Vertex& that) {
    this->x = that.x;
    this->y = that.y;
    this->z = that.z;
    return *this;
  }
  Vertex& operator+=(const Vertex& that) {
    this->x += that.x;
    this->y += that.y;
    this->z += that.z;
    return *this;
  }
  Vertex& operator-=(const Vertex& that) {
    this->x -= that.x;
    this->y -= that.y;
    this->z -= that.z;
    return *this;
  }
  Vertex& operator*=(const float that) {
    this->x *= that;
    this->y *= that;
    this->z *= that;
    return *this;
  }
  Vertex& operator/=(const float that) {
    this->x /= that;
    this->y /= that;
    this->z /= that;
    return *this;
  }
};

Vertex operator+(const Vertex& a, const Vertex& b) {
  Vertex v{a.x + b.x, a.y + b.y, a.z + b.z};
  return v;
}

Vertex operator-(const Vertex& a, const Vertex& b) {
  Vertex v{a.x - b.x, a.y - b.y, a.z - b.z};
  return v;
}

Vertex operator*(const Vertex& a, const float b) {
  Vertex v{a.x * b, a.y * b, a.z * b};
  return v;
}

struct State {
  float x;
  float r[4];
  Vertex position[N];
};

bool load(string fileName, State& state, vector<unsigned short>& triangleIndex,
          vector<unsigned short>& lineIndex,
          vector<vector<unsigned short> >& neighbor) {
  ifstream file(fileName);
  if (!file.is_open()) return false;

  string line;
  int n = 0;
  int readState = 0;
  while (getline(file, line)) {
    if (line == "|") {
      readState++;
      continue;
    }
    switch (readState) {
      case 0: {
        vector<float> v;
        stringstream ss(line);
        float f;
        while (ss >> f) {
          v.push_back(f);
          if (ss.peek() == ',') ss.ignore();
        }
        state.position[n].x = v[0];
        state.position[n].y = v[1];
        state.position[n].z = v[2];
        n++;
        // cout << v[0] << "|" << v[1] << "|" << v[2] << endl;
      } break;

      case 1: {
        stringstream ss(line);
        int i;
        if (ss >> i) {
          triangleIndex.push_back(i);

          static int c = 0, last;
          if (c % 3) lineIndex.push_back(last);
          lineIndex.push_back(i);
          last = i;
          c++;
        } else
          return false;
        // cout << i << endl;
      } break;

      case 2: {
        vector<unsigned short> v;
        stringstream ss(line);
        int i;
        while (ss >> i) {
          v.push_back(i);
          if (ss.peek() == ',') ss.ignore();
        }
        if ((v.size() != 5) && (v.size() != 6)) return false;
        neighbor.push_back(v);
      } break;
    }
  }
  file.close();

  return true;
}
#endif
