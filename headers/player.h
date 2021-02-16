#pragma once
#include "object.h"
#include "terrain.h"

class Player : public Object {
public:
	void move(Terrain* terrain);
	void rotate(vec2 cursor);
	void jump();
	bool mov[6], rot[6];
	vec2 viewAngle;
	vec3 up, right, direction;
	mat4 View;
	Player();
	~Player();
};

