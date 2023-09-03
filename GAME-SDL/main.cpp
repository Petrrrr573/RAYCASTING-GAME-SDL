#include <SDL.h>
#include <SDL_image.h>
#include <iostream> 
#include <cmath>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800

#define PI 3.1415926535

#define PI2 PI/2
#define PI3 3*PI/2

#define RAD 0.0174532925 // One degree in radiants


// game Class
class Game {
public:
	int mapX = 16, mapY = 16, mapSize = 256;

	int map[256] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,1,1,1,2,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,
		1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,
		1,0,0,1,0,0,0,0,0,0,2,2,2,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	bool isRunning;
	int tilleWidth;

	SDL_Window* window;
	SDL_Renderer* renderer;

	int lastFrame, frameCount, fps;

	int timerFPS;

	void MakeWindow(const char* name) {
		isRunning = true;

		window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * 2, HEIGHT, 0);

		renderer = SDL_CreateRenderer(window, -1, 0);


		SDL_GetRendererOutputSize(renderer, &mapSize, &mapSize); // Gets the width of the screen

		tilleWidth = WIDTH / mapX; // Gets the width of a tille
	}

	// Draws the minimap
	void DrawMap() {
		int x, y;

		// Draw the map tiles
		for (y = 0; y < mapY; y++) {
			for (x = 0; x < mapX; x++) {
				if (map[y * mapX + x] == 1) {
					SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
					SDL_Rect rect = { x * (mapSize / mapX) + 1, y * (mapSize / mapY) + 1, mapSize / mapX - 1, mapSize / mapY - 1 };
					SDL_RenderFillRect(renderer, &rect); 
				}

				if (map[y * mapX + x] == 2) {
					SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
					SDL_Rect rect = { x * (mapSize / mapX) + 1, y * (mapSize / mapY) + 1, mapSize / mapX - 1, mapSize / mapY - 1 };
					SDL_RenderFillRect(renderer, &rect);
				}
			}
		}

		// Draw the horizontal grid lines
		for (y = 0; y < mapY; y++) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderDrawLine(renderer, 0, y * (mapSize / mapY), mapSize, y * (mapSize / mapY));
		}

		// Draw the vertical grid lines
		for (x = 0; x < mapX; x++) {
			SDL_RenderDrawLine(renderer, x * (mapSize / mapX), 0, x * (mapSize / mapX), mapSize);
		}

	}

	// Limiting the FPS
	void HandleFps() {
		lastFrame = SDL_GetTicks();
		if (lastFrame >= (lastFrame + 1000)) {
			static int lastTime = lastFrame;
			fps = frameCount;
			frameCount = 0;

		}
		frameCount++;
		timerFPS = SDL_GetTicks() - lastFrame;
		if (timerFPS < (1000 / 60)) {
			SDL_Delay((1000 / 60) - timerFPS);
		}
	}
	
	// Gets the distance of a ray
	double distance(double playerX, double playerY, double rayX, double rayY, double rayAngle, double playerAngle) {
		double distance = 0;

		double xDifference = rayX - playerX;
		double yDifference = rayY - playerY;

		distance = xDifference * cos(playerAngle) + yDifference * sin(playerAngle);

		return distance;
	}

	void raycasting(double xPos, double yPos, double playerAngle, int& currentFrame) {
		double rayY = -1, rayX = -1;

		double horizontalY = 0, horizontalX = 0; // x, y possitions of horizontal rays
		double verticalY = 0, verticalX = 0; // x, y possitions of vertical rays
		double horizontalDistance = 0, verticalDistance = 0; // distances of final rays

		double finalDistance = 0; // Final distance

		double xOffset = 0, yOffset = 0;

		double playerX = xPos + 16;
		double playerY = yPos + 16;

		double dx = 0, dy = 0; // x, y possitions in a tille

		int mx, my, mp; // possition on a map

		int dof; // depth of field

		double rayAngle = playerAngle;

		int rays = 100; // number of rays

		rayAngle -= rays / 2 * RAD;


		//Drawing floor
		SDL_SetRenderDrawColor(renderer, 200, 20, 20, 255);
		SDL_Rect rect = { WIDTH, 800, WIDTH, -HEIGHT / 2 };
		SDL_RenderFillRect(renderer, &rect);


		for (int r = 0; r < rays; r++) {
			bool horizontalHit;
			if (rayAngle < 0) {
				rayAngle += 2 * PI;
			}
			if (rayAngle > 2 * PI) {
				rayAngle -= 2 * PI;
			}
			/* --- Check horizontal lines--- */
			dof = 0;
			double aTan = -1 / tan(rayAngle);

			// looking straight left or right
			if (rayAngle <= 0 || rayAngle == PI) {
				rayX = playerX; rayY = playerY;
				dof = 30;
			}

			// looking up
			if (rayAngle > PI) {
				rayY = floor(playerY / tilleWidth) * tilleWidth - 1;
				dy = playerY - rayY;
				rayX = (dy)*aTan + playerX;
				yOffset = -tilleWidth;
				xOffset = -yOffset * aTan;
			}

			// looking down
			if (rayAngle < PI) {
				rayY = floor(playerY / tilleWidth) * tilleWidth + tilleWidth;
				dy = playerY - rayY;
				rayX = (dy)*aTan + playerX;
				yOffset = tilleWidth;
				xOffset = -yOffset * aTan;
			}


			while (dof < 30) {
				mx = int(floor(rayX / tilleWidth));
				my = int(floor(rayY / tilleWidth));
				mp = my * mapX + mx;

				// hit wall
				if (mp > 0 && mp < mapX * mapY && map[mp] != 0) {
					dof = 30;
					break;
				}
				else {
					dof++;
					rayX += xOffset;
					rayY += yOffset;
				}
			}

			horizontalY = rayY, horizontalX = rayX;
			horizontalDistance = distance(playerX, playerY, horizontalX, horizontalY, rayAngle, playerAngle);

			/* --- Check vertical lines--- */

			dof = 0;
			double nTan = -tan(rayAngle);

			// looking straight up or down
			if (rayAngle == PI3 || rayAngle == PI2) {
				rayX = playerX; rayY = playerY;
				dof = 30;
			}

			// looking right
			if (rayAngle < PI2 || rayAngle > PI3) {
				rayX = floor(playerX / tilleWidth) * tilleWidth + tilleWidth;
				dx = playerX - rayX;
				rayY = (dx)*nTan + playerY;
				xOffset = tilleWidth;
				yOffset = -xOffset * nTan;
			}

			// looking left
			if (rayAngle > PI2 && rayAngle < PI3) {
				rayX = floor(playerX / tilleWidth) * tilleWidth - 1;
				dx = playerX - rayX;
				rayY = (dx)*nTan + playerY;
				xOffset = -tilleWidth;
				yOffset = -xOffset * nTan;
			}

			while (dof < 30) {
				mx = int(floor(rayX / tilleWidth));
				my = int(floor(rayY / tilleWidth));
				mp = my * mapX + mx;

				// hit wall
				if (mp > 0 && mp < mapX * mapY && map[mp] != 0) {
					dof = 30;
					break;
				}
				else {
					dof++;
					rayX += xOffset;
					rayY += yOffset;
				}
			}

			verticalY = rayY, verticalX = rayX;
			verticalDistance = distance(playerX, playerY, verticalX, verticalY, rayAngle, playerAngle);

			// Get shorter distance
			if (horizontalDistance > verticalDistance) {
				rayX = verticalX;
				rayY = verticalY;
				finalDistance = verticalDistance;
				horizontalHit = false;
			}
			else if (horizontalDistance < verticalDistance) {
				rayX = horizontalX;
				rayY = horizontalY;
				finalDistance = horizontalDistance;
				horizontalHit = true;
			}
			else {
				// Both distances are the same; handle corner/edge case
				if (rayAngle >= 0 && rayAngle < PI) {
					// Ray is facing down, prefer vertical hit
					rayX = horizontalX;
					rayY = horizontalY;
					finalDistance = horizontalDistance;
					horizontalHit = true;
				}
				else {
					// Ray is facing up, prefer horizontal hit
					rayX = verticalX;
					rayY = verticalY;
					finalDistance = verticalDistance;
					horizontalHit = false;
				}
			}

			//draws the rays on the minimap
			if (rayX >= 0 && rayX <= 800) {
				if (rayY >= 0 && rayY <= 800) {
					mx = int(floor(rayX / tilleWidth));
					my = int(floor(rayY / tilleWidth));
					mp = my * mapX + mx;

					// hit wall
					if (mp > 0 && mp < mapX * mapY) {
						if (map[mp] == 1) {
							SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
						}
						else if (map[mp] == 2) {
							SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
						}
						else {
							SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
						}
						SDL_RenderDrawLine(renderer, playerX, playerY, rayX, rayY);
					}

					// Draw 3D Walls
					double wh = (tilleWidth * HEIGHT) / finalDistance;
					if (wh > 1000) {
						wh = 1000;
					}

					double lineO = HEIGHT - wh / 1.5;

					SDL_Rect rect = { r * (800 / rays) + WIDTH, lineO / 2, 800 / rays, wh };
					if (mp > 0 && mp < mapX * mapY) {
						if (map[mp] == 1) {
							if (horizontalHit == true) {
								SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
							}
							else if (horizontalHit == false) {
								SDL_SetRenderDrawColor(renderer, 0, 0, 130, 255);
							}
							else { continue; }
						}
						else if (map[mp] == 2) {
							if (horizontalHit == true) {
								SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
							}
							else if (horizontalHit == false) {
								SDL_SetRenderDrawColor(renderer, 0, 130, 0, 255);
							}
						}
						SDL_RenderFillRect(renderer, &rect);
					}
				}
			}

			rayAngle += RAD;
		}
	}
};

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
