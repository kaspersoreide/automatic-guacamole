#include "sky.h"
#include "game.h"
#include "matrix.h"

Sky::Sky() {
    //construct vertex array with 6 vertices: 2 triangles covering the screen
    GLuint vertexBuffer;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	GLfloat vertices[] = {
		1.0,	-1.0,	0.9999999,	1.0,
		1.0,	1.0,	0.9999999,	1.0,
		-1.0,	1.0,	0.9999999,	1.0,
		-1.0,	-1.0,	0.9999999,	1.0,
		1.0,	-1.0,	0.9999999,	1.0,
		-1.0,	1.0,	0.9999999,	1.0
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    program = loadShaders("shaders/sky/vert.glsl", "shaders/sky/frag.glsl");

	sunAngle = vec2(0.0f, 0.0f);
}

void Sky::render(mat4 VP) {
	sunAngle = vec2(0.2f * game->time,  0.7f);
	glUseProgram(program);
	GLuint locVP = glGetUniformLocation(program, "VP");
	glUniformMatrix4fv(locVP, 1, GL_FALSE, &VP[0][0]);
	vec3 sun = sphereToCartesian(sunAngle);
	vec3 sight = game->player->direction;
	vec2 screenScale = vec2(game->aspect * sin(game->fovy / 2.0f), sin(game->fovy / 2.0f));
    glUniform3fv(glGetUniformLocation(program, "sunDir"), 1, &sun[0]);
	glUniform3fv(glGetUniformLocation(program, "viewDir"), 1, &sight[0]);
	glUniform3fv(glGetUniformLocation(program, "up"), 1, &game->player->up[0]);
    glUniform3fv(glGetUniformLocation(program, "right"), 1, &game->player->right[0]);
	glUniform2fv(glGetUniformLocation(program, "screenScale"), 1, &screenScale[0]);


	glBindVertexArray(vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}