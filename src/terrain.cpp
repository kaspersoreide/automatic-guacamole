#include "terrain.h"
#include <math.h>
#include <vector>

Terrain::Chunk::~Chunk() {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArray);
}

void Terrain::makeChunk(ivec2 pos) {
    
    Chunk* c = new Chunk();
    //chunk is square of length CHUNK_SIZE, 6 vertices per "tile"
    int numVert = CHUNK_SIZE * CHUNK_SIZE * 6;
    //8 floats per vertex;
    int stride = 8 * sizeof(float);
    glCreateBuffers(1, &c->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, c->vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, numVert * stride, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, c->vertexBuffer);
    glUseProgram(computeProgram);
    glUniform2i(glGetUniformLocation(computeProgram, "chunkPos"), pos[0], pos[1]);
	glDispatchCompute(CHUNK_SIZE, CHUNK_SIZE, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, c->vertexBuffer);
    glCreateVertexArrays(1, &c->vertexArray);
    glBindVertexArray(c->vertexArray);
    
    
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    
	glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	
    auto err = glGetError();
    if (err != GL_NO_ERROR) std::cout << "error: " << err << '\n';
    std::pair<ivec2, Chunk*> pair(pos, c);
	chunks.insert(pair);
    //print
    /*glBindBuffer(GL_ARRAY_BUFFER, c->vertexBuffer);
    //glBindVertexArray(c.vertexArray);
	std::vector<float> arr(numVert * stride / sizeof(float));
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, numVert*stride, &arr[0]);
	for (int i = 0; i < 400; i++) {
		if (i % 8 == 0) {
			std::cout << '\n';
		}
		std::cout << arr[i] << ", ";
	}
    std::cout << '\n';*/
}

Terrain::Terrain() {
    program = loadShaders("shaders/terrain/vert.glsl", "shaders/terrain/frag.glsl");
    computeProgram = loadComputeShader("shaders/terrain/compute.glsl");
}

void Terrain::render(mat4 VP) {
    glUseProgram(program);
    glUniformMatrix4fv(
        glGetUniformLocation(program, "VP"),
        1, GL_FALSE, &VP[0][0]
    );
    for (auto c : chunks) {
        glBindBuffer(GL_ARRAY_BUFFER, c.second->vertexBuffer);
        glBindVertexArray(c.second->vertexArray);
        //glUniform2i(glGetUniformLocation(program, "chunkPos"), c.first[0], c.first[1]);
    
        glDrawArrays(GL_TRIANGLES, 0, CHUNK_SIZE * CHUNK_SIZE * 6);
        //std::cout << "cock" << '\n';
    }
    auto err = glGetError();
    if (err != GL_NO_ERROR) std::cout << "error: " << err << '\n';
}

float Terrain::getHeight(vec2 pos) {
    ivec2 chunkPos(std::floor(pos.x), std::floor(pos.y));
    int x = std::floor(pos.x);
    int y = std::floor(pos.y);
    auto it = chunks.find(ivec2(x, y));
    if (it == chunks.end()) return 0.f;
    Chunk* c = it->second;

    glBindVertexArray(c->vertexArray);

    float dataArr[6 * 8];
    vec2 d(pos - vec2(chunkPos));
    ivec2 offset = ivec2(d * float(CHUNK_SIZE));
    vec2 lilbit = float(CHUNK_SIZE) * (d - vec2(offset));
    uint idx = 8 * 6 * (offset.x + CHUNK_SIZE * offset.y);
    //std::cout << "pos: " << pos.x << ", " << pos.y << "\n";
    //std::cout << "cpos: " << chunkPos.x << ", " << chunkPos.y << "\n";
    std::cout << "offset: " << offset.x << ", " << offset.y << "\n";
    std::cout << "offset float: " << d.x << ", " << d.y << "\n";

    glGetBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * idx, 4 * 8 * sizeof(float), dataArr);
    float h = 0.0f;
    for (int i = 0; i < 4; i++) {
        h += dataArr[8 * i + 1];
    }
    h /= 4;
    glBindVertexArray(0);
    //std::cout << h << '\n';
    return h;
}


void Terrain::update(vec3 playerPos) {
    ivec2 pos = ivec2(playerPos.x, playerPos.z) - ivec2(1, 1);
    //loop for deleting chunks far away
    auto it = chunks.begin();
    while (it != chunks.end()) {
        ivec2 offset = it->first - pos;
        int dist2 = offset.x*offset.x + offset.y*offset.y;
        if (dist2 > 2 * RENDER_DIST * RENDER_DIST) {
            delete it->second;
            it = chunks.erase(it);
            continue;
        }
        ++it;
    }
    //loop for creating new chunks
    for (int x = -RENDER_DIST; x <= RENDER_DIST; x++) {
        for (int y = -RENDER_DIST; y <= RENDER_DIST; y++) {
            if (x*x + y*y > RENDER_DIST * RENDER_DIST) continue;
            ivec2 chunkPos(pos.x + x, pos.y + y);
            it = chunks.find(chunkPos);
            if (it == chunks.end()) makeChunk(chunkPos);
        }
    }
}