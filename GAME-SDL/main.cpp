#include <iostream> 
#include <cmath>
#include <math.h>

#include "main.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "button.h"
#include "map_editor.h"



int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);
	Game game;

	// EDITOR PART
	game.MakeWindow("Map Editor", WIDTH - 200, HEIGHT);

	MapEditor editor(game.renderer);
	editor.isRunning = true;

	while (editor.isRunning) {
		game.HandleFps();
		game.Input(editor.isRunning);

		SDL_RenderClear(game.renderer);

		game.DrawMap(game.mapEditorTilleWidth); // Draws the map

		editor.update(game.mapX, game.mapY, game.mapSize, game.map, game.mapEditorTilleWidth, game.renderer);

		SDL_SetRenderDrawColor(game.renderer, SKY_COLOR); // Draws the background

		SDL_RenderPresent(game.renderer);
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);


	// GAME PART
	Player player(100,100);
	Enemy enemy(200,200);

	game.MakeWindow("Raycaster", WIDTH, HEIGHT);
	game.isRunning = true;

	game.minimapTilleWidth = game.minimapWidth / game.mapX;
	player.Update(game.renderer, game.minimapTilleWidth, game.mapX);
	enemy.Update(game.renderer, game.minimapTilleWidth, game.mapX);


	// Main loop
	while (game.isRunning) {
		game.HandleFps();

		player.Input(game.isRunning, game.tilleWidth, game.mapX, game.mapY, game.mapSize, game.map); // Movement, QUIT
		enemy.Input(game.isRunning, game.tilleWidth, game.mapX, game.mapY, game.mapSize, game.map); // Movement, QUIT
		game.Input(game.isRunning);

		SDL_RenderClear(game.renderer);

		game.raycasting(player.xPos, player.yPos, player.playerAngle, player.currentFrame, player.pWidthScaled);

		game.DrawMap(game.minimapTilleWidth); // Draws the map

		player.Draw(game.renderer, game.minimapTilleWidth); // Draws the player
		enemy.Draw(game.renderer, game.minimapTilleWidth); // Draws the player

		SDL_SetRenderDrawColor(game.renderer, SKY_COLOR); // Draws the background

		SDL_RenderPresent(game.renderer);
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;
}
	