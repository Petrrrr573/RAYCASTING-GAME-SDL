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
		game.HandleFps();
		game.Input(editor.isRunning);

		SDL_RenderClear(game.renderer);

		game.DrawMap(); // Draws the map

		editor.update(game.mapX, game.mapY, game.mapSize, game.map, game.tilleWidth);

		SDL_SetRenderDrawColor(game.renderer, SKY_COLOR); // Draws the background

		SDL_RenderPresent(game.renderer);
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);


	// GAME PART
	Player player;

	game.MakeWindow("Raycaster", WIDTH*2, HEIGHT, game.isRunning);

	player.Update(game.renderer, game.tilleWidth, game.mapX);


	// Main loop
	while (game.isRunning) {
		game.HandleFps();

		player.Input(game.isRunning, game.tilleWidth, game.mapX, game.mapY, game.mapSize, game.map); // Movement, QUIT
		game.Input(game.isRunning);

		SDL_RenderClear(game.renderer);

		game.DrawMap(); // Draws the map

		game.raycasting(player.xPos, player.yPos, player.playerAngle, player.currentFrame, player.pWidthScaled);

		player.Draw(game.renderer); // Draws the player

		SDL_SetRenderDrawColor(game.renderer, SKY_COLOR); // Draws the background

		SDL_RenderPresent(game.renderer);
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;
}
