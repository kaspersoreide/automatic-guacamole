#pragma once
#include "object.h"
class Player : public Object {
public:
	void move();
	bool mov[6], rot[6];
	Player();
	~Player();
};

