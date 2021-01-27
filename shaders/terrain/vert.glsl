#version 430

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex;

uniform mat4 VP;
uniform ivec2 chunkPos;

out vec3 color;

void main() {
    float shade = dot(normal, normalize(vec3(0.0, 1.0, 0.0))); 
    color = vec3(shade);
    gl_Position = VP * vec4(pos, 1.0);
}