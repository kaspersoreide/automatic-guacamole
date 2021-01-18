#include "rng.h"

unsigned int rng::s = 12;

unsigned int rng::hash(unsigned int x) {
    x += (x << 10u);
	x ^= (x >> 6u);
	x += (x << 3u);
	x ^= (x >> 11u);
	x += (x << 15u);
	return x;
}

void rng::srand(unsigned int _s) {
    s = _s;
}

float rng::floatConstruct(unsigned int x) {
    const unsigned int ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
	const unsigned int ieeeOne = 0x3F800000u; // 1.0 in IEEE binary32

	x &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
	x |= ieeeOne;                          // Add fractional part to 1.0

	//int to float 
	float f = *(float*)(&x);              // Range [1:2]
	return f - 1.0f;                      // Range [0:1]
}

int rng::randi() {
    srand(hash(s));
    return *(int*)(&s);
}

int rng::randi(int l, int h) {
    int x = randi();
    x %= h - l;
    return x + l;
}

float rng::randf() {
    srand(hash(s));
    return floatConstruct(s);
}

float rng::randf(float l, float h) {
    float f = randf();
    f *= h - l;
    return f + l;
}

glm::vec3 rng::randvec3() {
    glm::vec3 v(randf(-1.0f, 1.0f), randf(-1.0f, 1.0f), randf(-1.0f, 1.0f));
    return glm::normalize(v);
}