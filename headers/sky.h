#pragma once

#include "terrain.h"

class Sky {
public:
    GLuint program, vertexArray;
    Sky();
    void render(mat4 VP);
    vec2 sunAngle;
};