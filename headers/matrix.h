#pragma once
#include "glm/glm.hpp"

using namespace glm;

mat3 srotate(vec3 rot);

mat4 translateR(mat3 R, vec3 p);