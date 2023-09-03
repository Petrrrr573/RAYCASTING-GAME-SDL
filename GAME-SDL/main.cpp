#include <SDL.h>
#include <SDL_image.h>
#include <iostream> 
#include <cmath>
#include <math.h>

#include "game.h"

#define WIDTH 800
#define HEIGHT 800

#define PI 3.1415926535

#define PI2 PI/2
#define PI3 3*PI/2

#define RAD 0.0174532925 // One degree in radiants


// Player class
class Player {
public:
	// Variables to store the player's position
	double xPos = 100;
	double yPos = 100;
	double playerAngle;

	int playerSpeed = 6;

	const int playerWidth = 8;   // Width of each frame in the texture
	const int playerHeight = 8;  // Height of each frame in the texture
	const int playerFrameCount = 32; // Total number of frames in the texture
	const int framesPerRow = 32; // Number of frames per row in the texture

	const double anglePerFrame = static_cast<double>(360) / framesPerRow; // Number of degreses for each frame
	const double radPerFrame = PI * anglePerFrame / 180; // Number of radiants for each frame

	int currentFrame = 0;

	double pdx = cos(playerAngle) * 3;
	double pdy = sin(playerAngle) * 3;

	SDL_Surface* playerSurface;
	SDL_Texture* playerTexture;

	void Update(SDL_Renderer* renderer) {
		playerSurface = IMG_Load("C:\\Users\\Petr\\Desktop\\player.png");
		playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
		SDL_FreeSurface(playerSurface);
	}

	// Handles inputs
	void Input(bool& isRunning, int tilleWidth, int mapX, int mapY, int mapSize, int* map) {
		if (currentFrame >= 32) {
			currentFrame = 0;
		}
		else if (currentFrame < 0) {
			currentFrame = 31;
		}

		SDL_Event event;

		const Uint8* state = SDL_GetKeyboardState(NULL);

		SDL_PollEvent(&event);

		if (state[SDL_SCANCODE_ESCAPE]) {
			isRunning = false;
		}

		if (state[SDL_SCANCODE_A]) {
			currentFrame += 1;
			playerAngle -= radPerFrame / 2;
			if (playerAngle < 0) {
				playerAngle += 2 * PI;
			}
			pdx = cos(playerAngle) * 3;
			pdy = sin(playerAngle) * 3;
		}

		if (state[SDL_SCANCODE_D]) {
			currentFrame -= 1;
			playerAngle += radPerFrame / 2;
			if (playerAngle > 2 * PI) {
				playerAngle -= 2 * PI;
			}
			pdx = cos(playerAngle) * 3;
			pdy = sin(playerAngle) * 3;
		}

		if (state[SDL_SCANCODE_W]) {
			if (map[int((floor(yPos + pdy) / tilleWidth)) * mapX + int(floor((xPos) / tilleWidth))] == 0 && map[int((floor(yPos + pdy) / tilleWidth)) * mapX + int(floor((xPos + 32) / tilleWidth))] == 0 && map[int((floor(yPos + pdy + 32) / tilleWidth)) * mapX + int(floor((xPos + 32) / tilleWidth))] == 0 && map[int((floor(yPos + pdy + 32) / tilleWidth)) * mapX + int(floor((xPos) / tilleWidth))] == 0) {
				yPos += pdy;
			}
			if (map[int((floor(yPos) / tilleWidth)) * mapX + int(floor((xPos + pdx) / tilleWidth))] == 0 && map[int((floor(yPos) / tilleWidth)) * mapX + int(floor((xPos + pdx + 32) / tilleWidth))] == 0 && map[int((floor(yPos + 32) / tilleWidth)) * mapX + int(floor((xPos + pdx + 32) / tilleWidth))] == 0 && map[int((floor(yPos + 32) / tilleWidth)) * mapX + int(floor((xPos + pdx) / tilleWidth))] == 0) {
				xPos += pdx;
			}
		}

		if (state[SDL_SCANCODE_S]) {
			if (map[int((floor(yPos - pdy) / tilleWidth)) * mapX + int(floor((xPos) / tilleWidth))] == 0 && map[int((floor(yPos - pdy) / tilleWidth)) * mapX + int(floor((xPos + 32) / tilleWidth))] == 0 && map[int((floor(yPos - pdy + 32) / tilleWidth)) * mapX + int(floor((xPos + 32) / tilleWidth))] == 0 && map[int((floor(yPos - pdy + 32) / tilleWidth)) * mapX + int(floor((xPos) / tilleWidth))] == 0) {
				yPos -= pdy;
			}
			if (map[int((floor(yPos) / tilleWidth)) * mapX + int(floor((xPos - pdx) / tilleWidth))] == 0 && map[int((floor(yPos) / tilleWidth)) * mapX + int(floor((xPos - pdx + 32) / tilleWidth))] == 0 && map[int((floor(yPos + 32) / tilleWidth)) * mapX + int(floor((xPos - pdx + 32) / tilleWidth))] == 0 && map[int((floor(yPos + 32) / tilleWidth)) * mapX + int(floor((xPos - pdx) / tilleWidth))] == 0) {
				xPos -= pdx;
			}
		}
	}

	void Draw(SDL_Renderer* renderer) {

		// Calculate the source rectangle based on the current frame
		int srcX = round(currentFrame * playerWidth);
		int srcY = 0; // Since all frames are on a single row, y is always 0

		// Set up the source and destination rectangles
		SDL_Rect srcRect = { srcX, srcY, playerWidth, playerHeight };
		SDL_Rect destRect = { xPos, yPos, playerWidth * 4, playerHeight * 4 };

		// Render the player's current frame
		SDL_RenderCopy(renderer, playerTexture, &srcRect, &destRect);
	}
};

int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);

	Game game;
	Player player;

	game.MakeWindow("Game");

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
