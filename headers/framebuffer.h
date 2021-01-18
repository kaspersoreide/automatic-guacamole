#pragma once
#include "GL/glew.h"
#include "loadshaders.h"

class Framebuffer {
public:
	Framebuffer(GLsizei width, GLsizei height);
	void renderTextureToScreen();
	void bind(){ glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); }
	void unbind(){ glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	GLuint texture, depthbuffer, VAO, framebuffer, program;
};
