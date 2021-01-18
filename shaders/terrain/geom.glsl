#version 430

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in VS_OUT {
    vec2 v;
} gs_in[];

uniform sampler2D tex;
uniform ivec2 chunkPos;
uniform float offset;
uniform mat4 VP;

out vec4 c;

const vec3 light = normalize(vec3(1.0, 1.0, 0.0));

void main() {
    
    vec2 v = gs_in[0].v;
    vec2 wp = v + chunkPos;
    vec4 t0 = texture(tex, v);
    vec4 t1 = texture(tex, vec2(v.x + offset, v.y));
    vec4 t2 = texture(tex, vec2(v.x, v.y + offset));
    vec4 t3 = texture(tex, vec2(v.x + offset, v.y + offset));
    vec4 v0 = gl_in[0].gl_Position;
    vec4 v1 = VP * vec4(wp.x + offset, t1.a, wp.y, 1.0);
    vec4 v2 = VP * vec4(wp.x, t2.a, wp.y + offset, 1.0);
    vec4 v3 = VP * vec4(wp.x + offset, t3.a, wp.y + offset, 1.0);
   
    gl_Position = v0;
    c = vec4(dot(light, t0.xyz));
	EmitVertex();
	gl_Position = v1;
    c = vec4(dot(light, t1.xyz));
	EmitVertex();
    gl_Position = v2;
    c = vec4(dot(light, t2.xyz));
    EmitVertex();

    gl_Position = v3;
    c = vec4(dot(light, t3.xyz));
    EmitVertex();
    //gl_Position = v2;
    //EmitVertex();
    //gl_Position = v1;
    //EmitVertex();
	EndPrimitive();
}