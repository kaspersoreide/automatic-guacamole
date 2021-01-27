#include "player.h"


void Player::move(Terrain* terrain) {
	const float rotAmount = 0.003f;
	const float movAmount = 0.00087f;
	vec3 right = Rotation * vec3(1.0f, 0.0f, 0.0f);
	vec3 up = Rotation * vec3(0.0f, 1.0f, 0.0f);
	vec3 forward = Rotation * vec3(0.0f, 0.0f, 1.0f);
	vec3 dSpin(0.0f);
	if (rot[0]) dSpin += rotAmount * forward;
	if (rot[1]) dSpin -= rotAmount * forward;
	if (rot[2]) dSpin -= rotAmount * up;
	if (rot[3]) dSpin += rotAmount * up;
	if (rot[4]) dSpin -= rotAmount * right;
	if (rot[5]) dSpin += rotAmount * right;

	if (mov[0]) vel -= movAmount * forward;
	if (mov[1]) vel += movAmount * forward;
	if (mov[2]) vel -= movAmount * right;
	if (mov[3]) vel += movAmount * right;
	if (mov[4]) vel += movAmount * up;
	if (mov[5]) vel -= movAmount * up;
	//vel.y -= 0.0005f;
	float h = terrain->getHeight(vec2(pos.x, pos.z));
	//if (vel.y < 0.0f && pos.y < h) {
		//vel.y *= -0.1f;
		vel.y = 0.0f;
		pos.y = h + 0.1f;
	
	spin *= 0.94f;
	vel *= 0.99f;
	setSpin(spin + dSpin);
	Object::move();
}

Player::Player() {
	pos = vec3(0.0, 2.0, 0.0);
}

Player::~Player()
{
}
