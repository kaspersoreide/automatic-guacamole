#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include "loadshaders.h"
#include <vector>
#include "rng.h"

#define CHUNK_SIZE 32
#define RENDER_DIST 6

using namespace glm;

class Terrain {

struct Chunk {
    GLuint vertexBuffer, vertexArray;
    ~Chunk();
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
    void makeChunk(ivec2 pos);
    float getHeight(vec2 pos);
private:
    GLuint program, computeProgram;
    std::unordered_map<ivec2, Chunk*, HashFunction> chunks;
};

