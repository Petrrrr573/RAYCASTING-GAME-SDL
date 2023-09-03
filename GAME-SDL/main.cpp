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

	void HandleFps() {
	// Limiting the FPS
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
	
	double distance(double playerX, double playerY, double rayX, double rayY, double rayAngle, double playerAngle) {
		double distance = 0;

		double x = rayX - playerX;
		double y = rayY - playerY;

		distance = x * cos(playerAngle) + y * sin(playerAngle);

		return distance;
	}

	void raycasting(double xPos, double yPos, double playerAngle, int& currentFrame) {
		double rayY = -1, rayX = -1;

		double hy = 0, hx = 0;
		double vy = 0, vx = 0;
		double hd = 0, vd = 0;

		double fd = 0;

		double xo = 0, yo = 0;

		double playerX = xPos + 16;
		double playerY = yPos + 16;

		double dx = 0, dy = 0;

		int mx, my, mp;

		int dof;

		double rayAngle = playerAngle;

		int rays = 100;

		rayAngle -= rays / 2 * RAD;


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
			// Check horizontal lines
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
				yo = -tilleWidth;
				xo = -yo * aTan;
			}

			// looking down
			if (rayAngle < PI) {
				rayY = floor(playerY / tilleWidth) * tilleWidth + tilleWidth;
				dy = playerY - rayY;
				rayX = (dy)*aTan + playerX;
				yo = tilleWidth;
				xo = -yo * aTan;
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
					rayX += xo;
					rayY += yo;
				}
			}

			hy = rayY, hx = rayX;
			hd = distance(playerX, playerY, hx, hy, rayAngle, playerAngle);

			// Check vertical lines

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
				xo = tilleWidth;
				yo = -xo * nTan;
			}

			// looking left
			if (rayAngle > PI2 && rayAngle < PI3) {
				rayX = floor(playerX / tilleWidth) * tilleWidth - 1;
				dx = playerX - rayX;
				rayY = (dx)*nTan + playerY;
				xo = -tilleWidth;
				yo = -xo * nTan;
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
					rayX += xo;
					rayY += yo;
				}
			}

			vy = rayY, vx = rayX;
			vd = distance(playerX, playerY, vx, vy, rayAngle, playerAngle);

			if (hd > vd) {
				rayX = vx;
				rayY = vy;
				fd = vd;
				horizontalHit = false;
			}
			else if (hd < vd) {
				rayX = hx;
				rayY = hy;
				fd = hd;
				horizontalHit = true;
			}
			else {
				// Both distances are the same; handle corner/edge case
				// You can choose which wall to render based on ray direction
				if (rayAngle >= 0 && rayAngle < PI) {
					// Ray is facing down, prefer vertical hit
					rayX = hx;
					rayY = hy;
					fd = hd;
					horizontalHit = true;
				}
				else {
					// Ray is facing up, prefer horizontal hit
					rayX = vx;
					rayY = vy;
					fd = vd;
					horizontalHit = false;
				}
			}

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
					double wh = (tilleWidth * HEIGHT) / fd;
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


class Player {
public:
	double xPos = 100;
	double yPos = 100;
	double playerAngle;

	SDL_Rect player_rect = { 100,100,25,25 };
	int player_speed = 6;

	const int PLAYER_WIDTH = 8;   // Width of each frame in the texture
	const int PLAYER_HEIGHT = 8;  // Height of each frame in the texture
	const int PLAYER_FRAME_COUNT = 32; // Total number of frames in the texture
	const int FRAMES_PER_ROW = 32; // Number of frames per row in the texture

	const double ANGLE_PER_FRAME = static_cast<double>(360) / FRAMES_PER_ROW;
	const double RAD_PER_FRAME = PI * ANGLE_PER_FRAME / 180;

	int currentFrame = 0;

	// Variables to store the player's position
	double pdx = cos(playerAngle) * 3;
	double pdy = sin(playerAngle) * 3;

	SDL_Surface* playerSurface;
	SDL_Texture* playerTexture;

	void Update(SDL_Renderer* renderer) {
		playerSurface = IMG_Load("C:\\Users\\Petr\\Desktop\\player.png");
		playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
		SDL_FreeSurface(playerSurface);
	}

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
			playerAngle -= RAD_PER_FRAME / 2;
			if (playerAngle < 0) {
				playerAngle += 2 * PI;
			}
			pdx = cos(playerAngle) * 3;
			pdy = sin(playerAngle) * 3;
		}

		if (state[SDL_SCANCODE_D]) {
			currentFrame -= 1;
			playerAngle += RAD_PER_FRAME / 2;
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
		int srcX = round(currentFrame * PLAYER_WIDTH);
		int srcY = 0; // Since all frames are on a single row, y is always 0

		// Set up the source and destination rectangles
		SDL_Rect srcRect = { srcX, srcY, PLAYER_WIDTH, PLAYER_HEIGHT };
		SDL_Rect destRect = { xPos, yPos, PLAYER_WIDTH * 4, PLAYER_HEIGHT * 4 };

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


	while (game.isRunning) {
		game.HandleFps();

		player.Input(game.isRunning, game.tilleWidth, game.mapX, game.mapY, game.mapSize, game.map); // Movement, QUIT

		SDL_RenderClear(game.renderer);

		game.DrawMap(); // Draws the map

		player.Draw(game.renderer);

		game.raycasting(player.xPos, player.yPos, player.playerAngle, player.currentFrame);

		SDL_SetRenderDrawColor(game.renderer, 200, 200, 200, 255);

		SDL_RenderPresent(game.renderer);
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;
}
