#version 430
layout(local_size_x = 1, local_size_y = 1) in;

struct Data {
	float d[9];		//vec instead of float made some ugly padding
	//vec3 pos;
	//vec3 normal;
	//vec2 tex;
};

layout(std430, binding = 0) buffer OutBuf
{
	Data dout[];
};

const float PI = 3.14159265;
uniform int chunkSize;
uniform ivec2 chunkPos;

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash(uint x) {
	x += (x << 10u);
	x ^= (x >> 6u);
	x += (x << 3u);
	x ^= (x >> 11u);
	x += (x << 15u);
	return x;
}

// Compound versions of the hashing algorithm internet person whipped together.
uint hash(uvec2 v) { return hash(v.x ^ hash(v.y)); }
uint hash(uvec3 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z)); }
uint hash(uvec4 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct(uint m) {
	const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
	const uint ieeeOne = 0x3F800000u; // 1.0 in IEEE binary32

	m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
	m |= ieeeOne;                          // Add fractional part to 1.0

	float  f = uintBitsToFloat(m);       // Range [1:2]
	return f - 1.0;                        // Range [0:1]
}

// Pseudo-random value in half-open range [0:1].
float random(float x) { return floatConstruct(hash(floatBitsToUint(x))); }
float random(vec2  v) { return floatConstruct(hash(floatBitsToUint(v))); }
float random(vec3  v) { return floatConstruct(hash(floatBitsToUint(v))); }
float random(vec4  v) { return floatConstruct(hash(floatBitsToUint(v))); }

float valueNoise(vec2 v) {
	vec2 a = abs(v);
	vec2 d = fract(a);
	vec2 f = floor(a);
	vec2 c = ceil(a);
	float c01 = random(vec2(f.x, c.y));
	float c10 = random(vec2(c.x, f.y));
	float c00 = random(f);
	float c11 = random(c);
	return mix(mix(c00, c10, d.x), mix(c01, c11, d.x), d.y);
}

float getHeight(vec2 p) {
	float h = 0.0;
	for (int i = -4; i < 4; i++) {
		h += valueNoise(1.24 * pow(3.0, i) * p) / (1 + pow(4.0, i));
	}
	return h;
	//return 0.0;
}

float normsin(float x, float f) {
	return 0.5 * sin(f * x) + 0.5;
}

float normcos(float x, float f) {
	return 0.5 * cos(f * x) + 0.5;
}

void main() {
	uvec2 coords = gl_GlobalInvocationID.xy;
	vec2 lilstep = vec2(1.0 / chunkSize); 
	vec2 offset = vec2(coords) * lilstep;
	uint i = 6 * (coords.x + chunkSize * coords.y);
	vec2 pos = vec2(chunkPos) + offset;
	
	vec3 v[4] = {
		vec3(pos.x, 0.0, pos.y),
		vec3(pos.x, 0.0, pos.y + lilstep.y),
		vec3(pos.x + lilstep.x, 0.0, pos.y),
		vec3(pos.x + lilstep.x, 0.0, pos.y + lilstep.y)
	};
	/*
	if (coords.x == 0 || coords.x == chunkSize - 1) {
		if (mod(coords.x, 2) == 0) {
			v[2].y = 0.5 * (getHeight(pos) + getHeight(vec2(pos.x + 2.0 * lilstep.x, pos.y)));
			v[3].y = 0.5 * (getHeight(vec2(pos.x, pos.y + lilstep.y)) + getHeight(vec2(pos.x + 2.0 * lilstep.x, pos.y + lilstep.y)));
		} else {
			v[0].y = 0.5 * (getHeight(vec2(pos.x - lilstep.x, pos.y)) + getHeight(vec2(pos.x + lilstep.x, pos.y)));
			v[1].y = 0.5 * (getHeight(vec2(pos.x - lilstep.x, pos.y + lilstep.y)) + getHeight(vec2(pos.x + lilstep.x, pos.y + lilstep.y)));
		}
	}*/

	//else {
		for (int i = 0; i < 4; i++) {
			v[i].y = getHeight(v[i].xz);
		}
	//}

	vec3 n[2] = {
		normalize(cross(v[2] - v[0], v[1] - v[0])),
		normalize(cross(v[1] - v[3], v[2] - v[3]))
	};
	
	for (int j = 0; j < 6; j++) {
		int v_idx = (j < 3) ? j : 3 - (j - 3);
		int n_idx = (j < 3) ? 0 : 1;
		dout[i + j].d[0] = v[v_idx].x;
		dout[i + j].d[1] = v[v_idx].y;
		dout[i + j].d[2] = v[v_idx].z;

		dout[i + j].d[3] = n[n_idx].x;
		dout[i + j].d[4] = n[n_idx].y;
		dout[i + j].d[5] = n[n_idx].z;

		float h = v[v_idx].y;
		const vec3 color1 = vec3(0.1, 0.1, 0.1);
		const vec3 color2 = vec3(0.3, 0.4, 0.3);
		vec3 color = mix(color1, color2, h / 2.0);
		dout[i + j].d[6] = color.x;
		dout[i + j].d[7] = color.y;
		dout[i + j].d[8] = color.z;
	}
}