#pragma once
#include "object.h"
#include "terrain.h"
class Player : public Object {
public:
	void move(Terrain* terrain);
	bool mov[6], rot[6];
	Player();
	~Player();
};

