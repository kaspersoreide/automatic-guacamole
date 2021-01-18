#pragma once
#include "GL/glew.h"
#include <glm/glm.hpp>
#include "loadshaders.h"
#include "matrix.h"

using namespace glm;

class Object {
public:
	Object() {};
	Object(vec3 _pos);
	void move();
	void setSpin(vec3 _spin);
	void setVel(vec3 _vel) { vel = _vel; }
	vec3 getPos() { return pos; }
	mat4 getModel() { return Model; }
	mat4 getView() { return inverse(Model); }
	mat3 getRotation() { return Rotation; }
protected:
	vec3 pos = vec3(0.0f);
	vec3 spin = vec3(0.0f);
	vec3 vel = vec3(0.0f);
	mat3 Spin = mat3(1.0f);
	mat3 Rotation = mat3(1.0f);
	vec3 rot = vec3(0.0f);
	mat4 Model = mat4(1.0f);
};