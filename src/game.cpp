#include "game.h"

void Game::Init() {
    //game = new Game();
}

Game::Game() {
    player = new Player();
    terrain = new Terrain();
    sky = new Sky();
    fovy = 1.2f;
    aspect = 16.0f / 9.0f;
    Projection = perspective(
		fovy,
		aspect,
		0.01f,
		(float)RENDER_DIST
	);
    t0 = Time::now();
}

void Game::iterate() {
    //calculate new time value
    t1 = Time::now();
	fsec t = t1 - t0;
    time = t.count();
    
    player->move(terrain);
    mat4 VP = Projection * player->View;
    sky->render(VP);
	terrain->update(player->getPos());
    terrain->render(VP);
	
}

