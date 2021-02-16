#version 430

layout(location = 0) in vec4 pos;
out vec2 uv;

void main() {
    uv = pos.xy;
    gl_Position = pos;
}
