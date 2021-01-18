#include "glfunctions.h"

GLuint vao2x3f(vector<vec3>& vertices){
    GLuint VBO, VAO;
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//writing all the vertices into the buffer
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vec3) * vertices.size(),
		&vertices[0],
		GL_STATIC_DRAW
	);
 
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(float),
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(float),
		(void*)(3 * sizeof(float))
	);

	return VAO;
}

GLuint vao1x3f(vector<vec3>& vertices){
    GLuint VBO, VAO;
	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//writing all the vertices into the buffer
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(vec3) * vertices.size(),
		&vertices[0],
		GL_STATIC_DRAW
	);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	return VAO;
}