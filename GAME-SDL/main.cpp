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

#define RAD 0.0174532925

class Game {
public:
	int mapX = 16, mapY = 16, mapS = 256;

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
	int w_tille;

	SDL_Window* window;
	SDL_Renderer* renderer;

	int lastFrame, frameCount, fps;

	int timerFPS;

	void MakeWindow() {
		isRunning = true;

		window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * 2, HEIGHT, 0);

		renderer = SDL_CreateRenderer(window, -1, 0);


		SDL_GetRendererOutputSize(renderer, &mapS, &mapS); // Gets the width of the screen

		w_tille = WIDTH / mapX; // Gets the width of a tille
	}


	void DrawMap() {
		int x, y;

		// Draw the map tiles
		for (y = 0; y < mapY; y++) {
			for (x = 0; x < mapX; x++) {
				if (map[y * mapX + x] == 1) {
					SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
					SDL_Rect rect = { x * (mapS / mapX) + 1, y * (mapS / mapY) + 1, mapS / mapX - 1, mapS / mapY - 1 };
					SDL_RenderFillRect(renderer, &rect); 
				}

				if (map[y * mapX + x] == 2) {
					SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
					SDL_Rect rect = { x * (mapS / mapX) + 1, y * (mapS / mapY) + 1, mapS / mapX - 1, mapS / mapY - 1 };
					SDL_RenderFillRect(renderer, &rect);
				}
			}
		}

		// Draw the horizontal grid lines
		for (y = 0; y < mapY; y++) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderDrawLine(renderer, 0, y * (mapS / mapY), mapS, y * (mapS / mapY));
		}

		// Draw the vertical grid lines
		for (x = 0; x < mapX; x++) {
			SDL_RenderDrawLine(renderer, x * (mapS / mapX), 0, x * (mapS / mapX), mapS);
		}


		/*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &player_rect);*/
		//SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
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
	
	
};


class Player {
public:
	double xPos = 100;
	double yPos = 100;
	double pa;

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
	double pdx = cos(pa) * 3;
	double pdy = sin(pa) * 3;

	SDL_Surface* playerSurface;
	SDL_Texture* playerTexture;

	void Update(SDL_Renderer* renderer) {
		playerSurface = IMG_Load("C:\\Users\\Petr\\Desktop\\player.png");
		playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
		SDL_FreeSurface(playerSurface);
	}

	void Input(bool& isRunning, int w_tille, int mapX, int mapY, int mapS, int* map) {
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
			pa -= RAD_PER_FRAME / 2;
			if (pa < 0) {
				pa += 2 * PI;
			}
			pdx = cos(pa) * 3;
			pdy = sin(pa) * 3;
		}

		if (state[SDL_SCANCODE_D]) {
			currentFrame -= 1;
			pa += RAD_PER_FRAME / 2;
			if (pa > 2 * PI) {
				pa -= 2 * PI;
			}
			pdx = cos(pa) * 3;
			pdy = sin(pa) * 3;
		}

		if (state[SDL_SCANCODE_W]) {
			if (map[int((floor(yPos + pdy) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0 && map[int((floor(yPos + pdy) / w_tille)) * mapX + int(floor((xPos + 32) / w_tille))] == 0 && map[int((floor(yPos + pdy + 32) / w_tille)) * mapX + int(floor((xPos + 32) / w_tille))] == 0 && map[int((floor(yPos + pdy + 32) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0) {
				yPos += pdy;
			}
			if (map[int((floor(yPos) / w_tille)) * mapX + int(floor((xPos + pdx) / w_tille))] == 0 && map[int((floor(yPos) / w_tille)) * mapX + int(floor((xPos + pdx + 32) / w_tille))] == 0 && map[int((floor(yPos + 32) / w_tille)) * mapX + int(floor((xPos + pdx + 32) / w_tille))] == 0 && map[int((floor(yPos + 32) / w_tille)) * mapX + int(floor((xPos + pdx) / w_tille))] == 0) {
				xPos += pdx;
			}
		}

		if (state[SDL_SCANCODE_S]) {
			if (map[int((floor(yPos - pdy) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0 && map[int((floor(yPos - pdy) / w_tille)) * mapX + int(floor((xPos + 32) / w_tille))] == 0 && map[int((floor(yPos - pdy + 32) / w_tille)) * mapX + int(floor((xPos + 32) / w_tille))] == 0 && map[int((floor(yPos - pdy + 32) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0) {
				yPos -= pdy;
			}
			if (map[int((floor(yPos) / w_tille)) * mapX + int(floor((xPos - pdx) / w_tille))] == 0 && map[int((floor(yPos) / w_tille)) * mapX + int(floor((xPos - pdx + 32) / w_tille))] == 0 && map[int((floor(yPos + 32) / w_tille)) * mapX + int(floor((xPos - pdx + 32) / w_tille))] == 0 && map[int((floor(yPos + 32) / w_tille)) * mapX + int(floor((xPos - pdx) / w_tille))] == 0) {
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

double distance(double px, double py, double rx, double ry, double ra, double pa) {
	double distance = 0;

	double x = rx - px;
	double y = ry - py;

	distance = x * cos(pa) + y * sin(pa);

	return distance;
}

void raycasting(int mapX, int mapY, int mapS, int* map, double xPos, double yPos, double pa, SDL_Renderer* renderer, int w_tille, int& currentFrame) {
	double ry = -1, rx = -1;

	double hy = 0, hx = 0;
	double vy = 0, vx = 0;
	double hd = 0, vd = 0;

	double fd = 0;

	double xo = 0, yo = 0;

	double px = xPos + 16;
	double py = yPos + 16;

	double dx = 0, dy = 0;

	int mx, my, mp;

	int dof;

	double ra = pa;

	int rays = 100;

	ra -= rays / 2 * RAD;


	SDL_SetRenderDrawColor(renderer, 200, 20, 20, 255);
	SDL_Rect rect = { WIDTH, 800, WIDTH, -HEIGHT / 2 };
	SDL_RenderFillRect(renderer, &rect);

	for (int r = 0; r < rays; r++) {
		bool horizontalHit;
		if (ra < 0) {
			ra += 2 * PI;
		}
		if (ra > 2 * PI) {
			ra -= 2 * PI;
		}
		// Check horizontal lines
		dof = 0;
		double aTan = -1 / tan(ra);

		// looking straight left or right
		if (ra <= 0 || ra == PI) {
			rx = px; ry = py;
			dof = 30;
		}

		// looking up
		if (ra > PI) {
			ry = floor(py / w_tille) * w_tille - 1;
			dy = py - ry;
			rx = (dy)*aTan + px;
			yo = -w_tille;
			xo = -yo * aTan;
		}

		// looking down
		if (ra < PI) {
			ry = floor(py / w_tille) * w_tille + w_tille;
			dy = py - ry;
			rx = (dy)*aTan + px;
			yo = w_tille;
			xo = -yo * aTan;
		}


		while (dof < 30) {
			mx = int(floor(rx / w_tille));
			my = int(floor(ry / w_tille));
			mp = my * mapX + mx;

			// hit wall
			if (mp > 0 && mp < mapX * mapY && map[mp] != 0) {
				dof = 30;
				break;
			}
			else {
				dof++;
				rx += xo;
				ry += yo;
			}
		}

		hy = ry, hx = rx;
		hd = distance(px, py, hx, hy, ra, pa);

		// Check vertical lines

		dof = 0;
		double nTan = -tan(ra);

		// looking straight up or down
		if (ra == PI3 || ra == PI2) {
			rx = px; ry = py;
			dof = 30;
		}

		// looking right
		if (ra < PI2 || ra > PI3) {
			rx = floor(px / w_tille) * w_tille + w_tille;
			dx = px - rx;
			ry = (dx)*nTan + py;
			xo = w_tille;
			yo = -xo * nTan;
		}

		// looking left
		if (ra > PI2 && ra < PI3) {
			rx = floor(px / w_tille) * w_tille - 1;
			dx = px - rx;
			ry = (dx)*nTan + py;
			xo = -w_tille;
			yo = -xo * nTan;
		}

		while (dof < 30) {
			mx = int(floor(rx / w_tille));
			my = int(floor(ry / w_tille));
			mp = my * mapX + mx;

			// hit wall
			if (mp > 0 && mp < mapX * mapY && map[mp] != 0) {
				dof = 30;
				break;
			}
			else {
				dof++;
				rx += xo;
				ry += yo;
			}
		}

		vy = ry, vx = rx;
		vd = distance(px, py, vx, vy, ra, pa);

		if (hd > vd) {
			rx = vx;
			ry = vy;
			fd = vd;
			horizontalHit = false;
		}
		else if (hd < vd) {
			rx = hx;
			ry = hy;
			fd = hd;
			horizontalHit = true;
		}
		else {
			// Both distances are the same; handle corner/edge case
			// You can choose which wall to render based on ray direction
			if (ra >= 0 && ra < PI) {
				// Ray is facing down, prefer vertical hit
				rx = hx;
				ry = hy;
				fd = hd;
				horizontalHit = true;
			}
			else {
				// Ray is facing up, prefer horizontal hit
				rx = vx;
				ry = vy;
				fd = vd;
				horizontalHit = false;
			}
		}

		if (rx >= 0 && rx <= 800) {
			if (ry >= 0 && ry <= 800) {
				mx = int(floor(rx / w_tille));
				my = int(floor(ry / w_tille));
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
					SDL_RenderDrawLine(renderer, px, py, rx, ry);
				}

				// Draw 3D Walls
				double wh = (w_tille * HEIGHT) / fd;
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
						if (horizontalHit==true) {
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

		ra += RAD;
	}
}

int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);

	Game game;
	Player player;

	game.MakeWindow();

	player.Update(game.renderer);


	while (game.isRunning) {
		game.HandleFps();

		player.Input(game.isRunning, game.w_tille, game.mapX, game.mapY, game.mapS, game.map); // Movement, QUIT

		SDL_RenderClear(game.renderer);

		game.DrawMap(); // Draws the map

		player.Draw(game.renderer);


		/*SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderDrawLine(renderer, xPos + 16, yPos + 16, xPos + 16 + pdx*50, yPos + 16 + pdy*50);*/

		raycasting(game.mapX, game.mapY, game.mapS, game.map, player.xPos, player.yPos, player.pa, game.renderer, game.w_tille, player.currentFrame);

		SDL_SetRenderDrawColor(game.renderer, 200, 200, 200, 255);

		SDL_RenderPresent(game.renderer);
	}

	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	return 0;
}
