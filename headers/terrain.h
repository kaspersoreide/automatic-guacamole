#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include "loadshaders.h"
#include <vector>
#include "rng.h"

using namespace glm;

class Terrain {

struct Chunk {
    GLuint image;
    //level of detail, 1 initial
    int LOD;
};

class HashFunction { 
public: 
    size_t operator()(const ivec2& in) const
    { 
        return rng::hash(*(unsigned int*)&in[0] ^ rng::hash(*(unsigned int*)&in[1]));
    } 
}; 

public:
    Terrain();
    void render(mat4 VP);
    void update(vec3 playerPos);
    void addDetail(ivec2 pos);
    void removeDetail(ivec2 pos);
    void makeChunk(ivec2 pos, int LOD);
private:
    static constexpr int chunkSize = 16;
    static constexpr int LODs = 3;
    GLuint VAO[LODs];
    GLuint program, computeProgram;
    std::unordered_map<ivec2, Chunk, HashFunction> chunks;
};

