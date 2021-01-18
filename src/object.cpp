#include "object.h"

Object::Object(vec3 _pos) {
	pos = _pos;
}

void Object::move() {
	//pos += 0.001f * cross(vec3(0.0f, 1.0f, 0.0f), pos);
	pos += vel;
	rot += spin;
	Rotation = Spin * Rotation;
	//Model = translateR(Rotation, pos) * Scale;
	Model = translateR(Rotation, pos);
	//Model = translateR(mat3(1.0f), pos);
}

void Object::setSpin(vec3 _spin) {
	spin = _spin;
	Spin = srotate(_spin);
}