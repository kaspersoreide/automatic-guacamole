#version 430

layout(location = 0) in vec2 pos;
//layout(location = 1) in vec3 normal;

uniform mat4 VP;
uniform ivec2 chunkPos;
uniform sampler2D tex;

out VS_OUT {
    vec2 v;
} vs_out;

void main() { 
    vs_out.v = pos;
    vec4 data = texture(tex, vs_out.v);
    gl_Position = VP * vec4(pos.x + chunkPos.x, data.a, pos.y + chunkPos.y, 1.0);
}