#include "terrain.h"
#include <math.h>

void Terrain::makeChunk(ivec2 pos, int LOD) {
    Chunk c;
    c.LOD = LOD;
    int numVert = chunkSize * (1 << LOD);
    
    //compute shader puts normal in rgb and height in a
    glGenTextures(1, &c.image);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, c.image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //texture is 1 larger than numvert to access corner for triangle tesselation
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, numVert + 1, numVert + 1, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, c.image, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    
 
    glUseProgram(computeProgram);
    glUniform1f(glGetUniformLocation(computeProgram, "offset"), 1.0f / numVert);
    glUniform2i(glGetUniformLocation(computeProgram, "chunkPos"), pos[0], pos[1]);
    glUniform1i(glGetUniformLocation(computeProgram, "LOD"), LOD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDispatchCompute(numVert + 1, numVert + 1, 1);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
   
	chunks.insert(std::pair<ivec2, Chunk>(pos, c));
}

Terrain::Terrain() {
    for (int i = 0; i < LODs; i++) {
        GLuint VBO;
        std::vector<vec2> vertices;
        int numVert = chunkSize * std::pow(2, i);
        for (int i = 0; i < numVert; i++) {
            for (int j = 0; j < numVert; j++) {
                float x = (float)i / numVert;
                float y = (float)j / numVert;
                vertices.push_back(vec2(x, y));
            }
        }
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
	    glBufferData(
	    	GL_ARRAY_BUFFER,
	    	2 * sizeof(float) * vertices.size(),
	    	&vertices[0],
	    	GL_STATIC_DRAW
	    );
        glGenVertexArrays(1, &VAO[i]);
        glBindVertexArray(VAO[i]);
        glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
    program = loadGeometryShader("shaders/terrain/vert.shader", "shaders/terrain/geom.shader", "shaders/terrain/frag.shader");
    computeProgram = loadComputeShader("shaders/terrain/compute.shader");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            makeChunk(ivec2(i, j), 0);        
        }
    }
}

void Terrain::render(mat4 VP) {
    glUseProgram(program);
    glUniformMatrix4fv(
        glGetUniformLocation(program, "VP"),
        1, GL_FALSE, &VP[0][0]
    );
    glActiveTexture(GL_TEXTURE0);
    //glPointSize(5.0f);
    for (auto c : chunks) {
        glBindVertexArray(VAO[c.second.LOD]);
        glBindTexture(GL_TEXTURE_2D, c.second.image);
        glUniform2i(glGetUniformLocation(program, "chunkPos"), c.first[0], c.first[1]);
        
        int s = std::pow(2, c.second.LOD) * chunkSize;
        glUniform1f(glGetUniformLocation(program, "offset"), 1.0f / s);
        glDrawArrays(GL_POINTS, 0, s * s);
    }
}

void Terrain::update(vec3 playerPos) {
    const int renderDistance = 12;
    ivec2 pos = ivec2(playerPos.x, playerPos.z) - ivec2(1, 1);
    auto it = chunks.begin();
    while (it != chunks.end()) {
        ivec2 offset = it->first - pos;
        int dist = abs(offset.x) + abs(offset.y);
        if (dist > renderDistance) {
            glDeleteTextures(1, &it->second.image);
            it = chunks.erase(it);
            continue;
        }
        ++it;
    }
    for (int x = -renderDistance; x <= renderDistance; x++) {
        for (int y = -renderDistance; y <= renderDistance; y++) {
            ivec2 chunkPos(pos.x + x, pos.y + y);
            float d2 = 1.0f - (float)(abs(x) + abs(y)) / (2 * renderDistance);
            int d = d2 * d2 * d2 * LODs;
            //if (d < 4) {
            //    d = 5;
            //}
            //else if (d < 5) {
            //    d = 4;
            //}
            //else if (d < 6) {
            //    d = 1;
            //} else d = 0;
            it = chunks.find(chunkPos);
            if (it != chunks.end()) {
                if (it->second.LOD != d) {
                    chunks.erase(it);
                }
                else continue;
            }
            makeChunk(chunkPos, d);
        }
    }
}