#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "framebuffer.h"
#include "player.h"
#include "rng.h"
#include <chrono>
#include "terrain.h"

GLFWwindow* window;
Player* player;
bool closed = false;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_E) {
		if (action == GLFW_PRESS) player->rot[0] = true;
		if (action == GLFW_RELEASE) player->rot[0] = false;
	}
	if (key == GLFW_KEY_Q) {
		if (action == GLFW_PRESS) player->rot[1] = true;
		if (action == GLFW_RELEASE) player->rot[1] = false;
	}
	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) player->rot[2] = true;
		if (action == GLFW_RELEASE) player->rot[2] = false;
	}
	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) player->rot[3] = true;
		if (action == GLFW_RELEASE) player->rot[3] = false;
	}
	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) player->rot[4] = true;
		if (action == GLFW_RELEASE) player->rot[4] = false;
	}
	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) player->rot[5] = true;
		if (action == GLFW_RELEASE) player->rot[5] = false;
	}

	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) player->mov[0] = true;
		if (action == GLFW_RELEASE) player->mov[0] = false;
	}
	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) player->mov[1] = true;
		if (action == GLFW_RELEASE) player->mov[1] = false;
	}
	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) player->mov[2] = true;
		if (action == GLFW_RELEASE) player->mov[2] = false;
	}
	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) player->mov[3] = true;
		if (action == GLFW_RELEASE) player->mov[3] = false;
	}
	if (key == GLFW_KEY_LEFT_SHIFT) {
		if (action == GLFW_PRESS) player->mov[4] = true;
		if (action == GLFW_RELEASE) player->mov[4] = false;
	}
	if (key == GLFW_KEY_LEFT_CONTROL) {
		if (action == GLFW_PRESS) player->mov[5] = true;
		if (action == GLFW_RELEASE) player->mov[5] = false;
	}
	if (key == GLFW_KEY_ESCAPE) {
		closed = true;
	}
}

int main() {
	rng::srand(425);
    glfwInit();
	window = glfwCreateWindow(1280, 720, "application", NULL, NULL);
	glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glewInit();
    glfwSetKeyCallback(window, keyCallback);
	glEnable(GL_DEPTH_TEST);

	player = new Player();
	Terrain terrain;
    
    mat4 Projection = perspective(
		1.2f,
		16.0f / 9,
		0.01f,
		100.0f
	);

    while (!glfwWindowShouldClose(window) && !closed) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //main loop
        player->move();
        mat4 VP = Projection * player->getView();
		terrain.update(player->getPos());
        terrain.render(VP);
        glfwSwapBuffers(window);
		glfwPollEvents();
    }

    return 0;
}