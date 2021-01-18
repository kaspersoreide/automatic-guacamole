#version 430
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D img_output;

const float PI = 3.14159265;

uniform float offset;
uniform ivec2 chunkPos;
uniform int LOD;

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
	vec2 a = v;
	vec2 d = fract(a);
	vec2 f = floor(a) + 1000000;
	vec2 c = ceil(a) + 1000000;
	float c01 = random(vec2(f.x, c.y));
	float c10 = random(vec2(c.x, f.y));
	float c00 = random(f);
	float c11 = random(c);
	return mix(mix(c00, c10, d.x), mix(c01, c11, d.x), d.y);
}

float getHeight(vec2 p) {
	float h = 0.0;
	for (int i = 0; i < 4 * LOD + 2; i++) {
		h += 0.6 * valueNoise(2.24 * pow(2, i) * p) / (1 + pow(2, i));
	}
	return h;
	//return 0.0;
}

void main() {
	ivec2 dims = imageSize(img_output) - ivec2(1);
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	vec2 pos = chunkPos + vec2(pixel_coords) / dims;
	vec3 v0 = vec3(pos.x, getHeight(pos), pos.y);
	vec3 v1 = vec3(pos.x + offset, getHeight(vec2(pos.x + offset, pos.y)), pos.y);
	vec3 v2 = vec3(pos.x, getHeight(vec2(pos.x, pos.y + offset)), pos.y + offset);
	vec3 n = normalize(cross(v2 - v0, v1 - v0));
	vec4 pixel = vec4(n, v0.y);
	imageStore(img_output, pixel_coords, pixel);
}