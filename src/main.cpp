#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "framebuffer.h"
#include "player.h"
#include "rng.h"
#include <chrono>
#include "terrain.h"
#include "sky.h"
#include "game.h"

GLFWwindow* window;
bool closed = false;
vec2 cursor(0.0f);
Game* game;

void cursorCallback(GLFWwindow* window, double x, double y) {
	vec2 _cursor(x, y);
	vec2 d = cursor - _cursor;
	game->player->rotate(d);
	cursor = _cursor;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) game->player->mov[0] = true;
		if (action == GLFW_RELEASE) game->player->mov[0] = false;
	}
	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) game->player->mov[1] = true;
		if (action == GLFW_RELEASE) game->player->mov[1] = false;
	}
	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) game->player->mov[2] = true;
		if (action == GLFW_RELEASE) game->player->mov[2] = false;
	}
	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) game->player->mov[3] = true;
		if (action == GLFW_RELEASE) game->player->mov[3] = false;
	}
	if (key == GLFW_KEY_LEFT_SHIFT) {
		if (action == GLFW_PRESS) game->player->mov[4] = true;
		if (action == GLFW_RELEASE) game->player->mov[4] = false;
	}
	if (key == GLFW_KEY_LEFT_CONTROL) {
		if (action == GLFW_PRESS) game->player->mov[5] = true;
		if (action == GLFW_RELEASE) game->player->mov[5] = false;
	}
	if (key == GLFW_KEY_ESCAPE) {
		closed = true;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		game->player->jump(game->terrain);
	}
}

int main() {
	
	rng::srand(425);
    glfwInit();
	window = glfwCreateWindow(1920, 1080, "application", glfwGetPrimaryMonitor(), NULL);
	glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glewInit();
    glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable( GL_BLEND );  
	glClearColor(0.0,0.0,0.0,0.0);
	game = new Game();
   
	//terrain--makeChunk(ivec2(0, 0));
	
    while (!glfwWindowShouldClose(window) && !closed) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //main loop
		game->iterate();
        glfwSwapBuffers(window);
		glfwPollEvents();
    }
	
    return 0;
}