#pragma once
#include "terrain.h"
#include "player.h"
#include "sky.h"
#include <chrono>
#define PI 3.14159265359F

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float> fsec;
typedef std::chrono::time_point<Time> Timepoint;

class Game {
public:
    static void Init();
    Game();
    void iterate();
    Player* player;
    Terrain* terrain;
    Sky* sky;
    mat4 Projection;
    float fovy, aspect;
    float time;
    Timepoint t0, t1;
};

extern Game* game;