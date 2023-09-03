#include <iostream> 
#include <cmath>
#include <math.h>

#include "main.h"
#include "game.h"
#include "player.h"


int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);

	Game game;
	Player player;

	game.MakeWindow("Raycaster");

	player.Update(game.renderer);


	// Main loop
	while (game.isRunning) {
		game.HandleFps();

		player.Input(game.isRunning, game.tilleWidth, game.mapX, game.mapY, game.mapSize, game.map); // Movement, QUIT

		SDL_RenderClear(game.renderer);

		game.DrawMap(); // Draws the map

		player.Draw(game.renderer); // Draws the player

		game.raycasting(player.xPos, player.yPos, player.playerAngle, player.currentFrame);

		SDL_SetRenderDrawColor(game.renderer, 200, 200, 200, 255); // Draws the background

		SDL_RenderPresent(game.renderer);
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;
}
