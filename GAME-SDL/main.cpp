#include <iostream> 
#include <cmath>
#include <math.h>

#include "main.h"
#include "game.h"
#include "player.h"
#include "map_editor.h"



int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	Game game;
	MapEditor editor;

	// EDITOR PART
	game.MakeWindow("Map Editor", WIDTH+400, HEIGHT, editor.isRunning);
	while (editor.isRunning) {
		game.Input(editor.isRunning);

		SDL_RenderClear(game.renderer);

		game.DrawMap(); // Draws the map

		editor.update(game.map, game.mapX, game.mapY, game.mapSize);

		SDL_SetRenderDrawColor(game.renderer, SKY_COLOR); // Draws the background

		SDL_RenderPresent(game.renderer);
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);


	// GAME PART
	Player player;

	game.MakeWindow("Raycaster", WIDTH*2, HEIGHT, game.isRunning);

	player.Update(game.renderer);


	// Main loop
	while (game.isRunning) {
		game.HandleFps();

		player.Input(game.isRunning, game.tilleWidth, game.mapX, game.mapY, game.mapSize, game.map, player.playerSpeed); // Movement, QUIT
		game.Input(game.isRunning);

		SDL_RenderClear(game.renderer);

		game.DrawMap(); // Draws the map

		game.raycasting(player.xPos, player.yPos, player.playerAngle, player.currentFrame);

		player.Draw(game.renderer); // Draws the player

		SDL_SetRenderDrawColor(game.renderer, SKY_COLOR); // Draws the background

		SDL_RenderPresent(game.renderer);
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;
}
