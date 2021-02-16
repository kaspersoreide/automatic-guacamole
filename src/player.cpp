#include "player.h"
#include "game.h"

void Player::move(Terrain* terrain) {
	const float speed = 0.0001f;
	direction = vec3(
		cos(viewAngle.y) * sin(viewAngle.x), 
		sin(viewAngle.y),
		cos(viewAngle.y) * cos(viewAngle.x)
	);
	// Right vector
	right = vec3(
		sin(viewAngle.x - 3.14f/2.0f), 
		0,
		cos(viewAngle.x - 3.14f/2.0f)
	);
	// Up vector
	up = cross( right, direction );
	if (mov[0]) vel += speed * direction;
	if (mov[1]) vel -= speed * direction;
	if (mov[2]) vel -= speed * right;
	if (mov[3]) vel += speed * right;
	//if (mov[4]) vel += speed * up;
	//if (mov[5]) vel -= speed * up;
	vel.y -= 0.0004f;
	float h = terrain->getHeight(vec2(pos.x, pos.z));
	if (vel.y <= 0.0f && pos.y <= h + 0.06f) {
		vel.y *= -0.01f;
		pos.y = h + 0.06f;
	}
	
	//spin *= 0.94f;
	vel.x *= 0.97f;
	vel.z *= 0.97f;
	//setSpin(spin + dSpin);
	//Object::move();
	pos += vel;
	View = lookAt(pos, pos + direction, up);
}

void Player::rotate(vec2 cursor) {
	const float cursorSpeed = 0.0016f;
	viewAngle += cursorSpeed * cursor;
	if (viewAngle.y < -PI / 2.0f) viewAngle.y = -PI / 2.0f;
	if (viewAngle.y > PI / 2.0f) viewAngle.y = PI / 2.0f;
}

Player::Player() {
	pos = vec3(0.0, 2.0, 0.0);
	viewAngle = vec2(0.0f, 2.14f / 2.0f);
}

void Player::jump() {
	vel.y = 0.01f;
}

Player::~Player()
{
}
