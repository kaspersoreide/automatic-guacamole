#version 430

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

uniform mat4 VP;
uniform float renderDist;
uniform vec3 sunDir;
uniform float t;
uniform vec3 playerPos;

out vec3 vcolor;

void main() {
    float shade = max(dot(normal, -sunDir), 0.2); 
    vec3 dpos = pos - playerPos;
    float d2 = dpos.x*dpos.x + dpos.z*dpos.z;
    vec3 npos = vec3(pos.x, pos.y + 0.1 * sin(0.3 * t) * d2, pos.z);
    gl_Position = VP * vec4(npos, 1.0);
    float fade = exp(-gl_Position.z * 0.4);
    vec3 fogColor = vec3(0.2, 0.0, 0.0);
    vcolor = mix(fogColor, shade * color, fade);
}