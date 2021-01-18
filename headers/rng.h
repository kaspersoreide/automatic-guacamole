#pragma once
#include "glm/glm.hpp"


namespace rng {
    extern unsigned int s;
    void srand(unsigned int _s);
    unsigned int hash(unsigned int x);
    float floatConstruct(unsigned int x);
    int randi();
    int randi(int l, int h);
    float randf();
    float randf(float l, float h);
    glm::vec3 randvec3();
}