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


int mapX = 16, mapY = 16, mapS = 256;

int map[] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
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

double xPos = 100;
double yPos = 100;
double pdx, pdy, pa;

int lastFrame, frameCount, fps;

void DrawMap(SDL_Renderer* renderer, int mapX, int mapY, int mapS, int* map, SDL_Rect player_rect) {
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

void Input(bool& isRunning, int player_speed, int w_tille, int mapX, int mapY, int mapS, int* map, double& xPos, double& yPos, double& pdx, double& pdy, double& pa, int PLAYER_WIDTH, int PLAYER_HEIGHT, int& currentFrame, double RAD_PER_FRAME) {
	if (currentFrame >= 32) {
		currentFrame = 0;
	} else if (currentFrame < 0) {
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
		pa -= RAD_PER_FRAME/2;
		if (pa < 0) {
			pa += 2 * PI;
		}
		pdx = cos(pa) * 3;
		pdy = sin(pa) * 3;
	}

	if (state[SDL_SCANCODE_D]) {
		currentFrame -= 1;
		pa += RAD_PER_FRAME/2;
		if (pa > 2 * PI) {
			pa -= 2 * PI;
		}
		pdx = cos(pa) * 3;
		pdy = sin(pa) * 3;
	}

	if (state[SDL_SCANCODE_W]) {
		if (map[int((floor(yPos + pdy) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0 && map[int((floor(yPos + pdy) / w_tille)) * mapX + int(floor((xPos + 32) / w_tille))] == 0 && map[int((floor(yPos + pdy+32) / w_tille)) * mapX + int(floor((xPos+32) / w_tille))] == 0 && map[int((floor(yPos + pdy+32) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0) {
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

	double dx=0, dy = 0;

	int mx, my, mp;

	int dof;

	double ra = pa;

	bool horizontalHit = false;

	int rays = 100;

	ra -= rays/2*RAD;
	for (int r = 0; r < rays; r++) {
		ra += RAD;
		if (ra < 0) {
			ra += 2 * PI;
		}
		if (ra > 2 * PI) {
			ra -= 2 * PI;
		}
		// Check horizontal lines
		dof = 0;
		double aTan = -1 /tan(ra);

		// looking straight left or right
		if (ra <= 0 || ra == PI) {
			rx = px; ry = py;
			dof = 8;
		}

		// looking up
		if (ra > PI) {
			ry = floor(py / w_tille) * w_tille-1;
			dy = py - ry;
			rx = (dy) * aTan + px;
			yo = -w_tille;
			xo = -yo * aTan;
		}

		// looking down
		if (ra < PI) {
			ry = floor(py / w_tille) * w_tille + w_tille;
			dy = py - ry;
			rx = (dy) * aTan + px;
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
			dof = 8;
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

		//std::cout << vd << " " << hd << std::endl;

		if (hd>vd){
			rx = vx;
			ry = vy;
			fd = vd;
		}
		else if (hd<vd) {
			rx = hx;
			ry = hy;
			fd = hd;
			horizontalHit = true;
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
			}
		}

		// Draw 3D Walls
		double wh = (w_tille * HEIGHT) / fd;
		if (wh > 800) {
			wh = 800;
		}

		double lineO = HEIGHT - wh / 2;
		
		SDL_Rect rect = {r* (800 / rays) + WIDTH, lineO/2, 800/rays, wh};
		if (mp > 0 && mp < mapX * mapY) {
			if (map[mp] == 1) {
				if (horizontalHit) {
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0, 0, 130, 255);
				}
			}
			else if (map[mp] == 2) {
				if (horizontalHit) {
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0, 130, 0, 255);
				}
			}
			SDL_RenderFillRect(renderer, &rect);
		}

		// looking left
		/*if (ra > PI2 && ra < PI3) {
			rx = floor(px / w_tille) * w_tille + w_tille;
			dx = px - rx;
			ry = (dx)*Tan + py;
			xo = -w_tille;
			yo = -xo * Tan;
		}*

		//// looking right
		//if (ra < PI2 || ra > PI3) {
		//	ry = floor(py / w_tille) * w_tille + w_tille;
		//	dy = py - ry;
		//	rx = (dy)*aTan + px;
		//	yo = w_tille;
		//	xo = -yo * aTan;
		//}


		//while (dof < 8) {
		//	mx = int(floor(rx / w_tille));
		//	my = int(floor(ry / w_tille));
		//	mp = my * mapX + mx;

		//	// hit wall
		//	if (mp > 0 && mp < mapX * mapY && map[mp] == 1) {
		//		dof = 8;
		//		break;
		//	}
		//	else {
		//		dof++;
		//		rx += xo;
		//		ry += yo;
		//	}
		//}
		//
		//if (rx >= 0 && rx <= 800) {
		//	if (ry >= 0 && ry <= 800) {
		//		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		//		SDL_RenderDrawLine(renderer, px, py, rx, ry);
		//		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		//		SDL_RenderDrawLine(renderer, px, py, px, ry);
		//		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		//		SDL_RenderDrawLine(renderer, px, ry, rx, ry);
		//	}
		//}


		//ra -= 0.0872664626 * 2;
		//for (int r = 0; r < 10; r++) {
		//	// Check horizontal lines
		//	dof = 0;
		//	double aTan = -1 / tan(ra);

		//	// looking up
		//	if (ra > PI) {
		//		ry = floor(py / w_tille) * w_tille;
		//		dy = py - ry;
		//		rx = (dy)*aTan + px;
		//		yo = -w_tille;
		//		xo = -yo * aTan;
		//	}

		//	// looking down
		//	//if (ra < PI) {
		//	//	ry = floor(py / w_tille) * w_tille + w_tille;
		//	//	dy = py - ry;
		//	//	rx = (dy) * aTan + px;
		//	//	yo = w_tille;
		//	//	xo = -yo * aTan;
		//	//}

		//	//// looking straight left or right
		//	//if (ra == 0 || ra == PI) {
		//	//	rx = xPos+16; ry = yPos+16;
		//	//	dof = 8;
		//	//}

		//	while (dof < 8) {
		//		mx = int(floor(rx / w_tille));
		//		my = int(floor(ry / w_tille));
		//		mp = my * mapX + mx;

		//		// hit wall
		//		if (mp > 0 && mp < mapX * mapY && map[mp] == 1) {
		//			std::cout << "zed" << std::endl;
		//			if (rx > 0 && rx < 800) {
		//				if (ry > 0 && ry < 800) {
		//					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		//					SDL_RenderDrawLine(renderer, xPos + 16, yPos + 16, rx, ry);
		//				}
		//			}
		//			dof = 8;
		//		}
		//		else {
		//			dof++;
		//			rx += xo;
		//			ry += yo;
		//		}

		//	}

		//	if (dof > 7) {
		//		if (rx > 0 && rx < 800) {
		//			if (ry > 0 && ry < 800) {
		//				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		//				SDL_RenderDrawLine(renderer, xPos + 16, yPos + 16, rx, ry);
		//			}
		//		}
		//	}

		//	ra += 0.0174532925 * 2;

		//// looking down
		//if (ra < PI) {
		//	ry = yPos - (floor((yPos+16) / w_tille+1) * w_tille) + w_tille;
		//	rx = (yPos+16 - ry) * aTan + xPos+16;
		//	yo = w_tille;
		//	xo = -yo * aTan;
		//}

		//// looking straight left or right
		//if (ra == 0 || ra == PI) {
		//	rx = xPos+16; ry = yPos+16;
		//	dof = 8;
		//}


		// Check vertical lines
		//dof = 0;
		//double nTan = -tan(ra);


		//// looking left
		//if (ra > PI2 && ra<PI3) {
		//	rx = xPos - (floor((xPos + 16) / w_tille) * w_tille) - 0.0001;
		//	ry = (xPos + 16 - ry) * nTan + yPos + 16;
		//	xo = -w_tille;
		//	yo = -xo * nTan;
		//}

		//// looking right
		//if (ra < PI2 || ra>PI3) {
		//	rx = xPos - (floor((xPos + 16) / w_tille) * w_tille) + w_tille;
		//	ry = (xPos + 16 - ry) * nTan + yPos + 16;
		//	xo = w_tille;
		//	yo = -xo * nTan;
		//}

		//// looking straight up or down
		//if (ra == 0 || ra == PI) {
		//	rx = xPos; ry = yPos;
		//	dof = 8;
		//}

		//while (dof < 8) {
		//	mx = int(floor(rx / w_tille));
		//	my = int(floor(ry / w_tille));
		//	mp = my * mapX + mx;

		//	// hit wall
		//	if (mp > 0 && mp < mapX * mapY && map[mp] == 1) {
		//		dof = 8;
		//	}
		//	else {
		//		rx += xo;
		//		ry += yo;
		//		dof++;
		//	}
		//}

		//if (rx < 0) {rx = 0;}
		//if (ry < 0) { ry = 0; }
		//if (rx > WIDTH) { rx = WIDTH; }
		//if (ry > HEIGHT) { ry = HEIGHT; }
		//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		//SDL_RenderDrawLine(renderer, xPos + 16, yPos + 16, rx, ry);



		/*float dx = 0;
		std::cout << la * 57.2957795 << std::endl;
		if (la > 0 && (la + 0.0174532925) * 57.2957795 < 90) {
			la += 0.0174532925;
			if (la != PI / 2 && la != 3 * PI / 2) {
				dx = dy / tan(la);
			}
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(renderer, xPos + 16, yPos + 16, xPos + dx, yPos - dy);
		}*/
	}

	/*for (int r = 0; r < 240; r++){
		float yo = 0, xo = 0;
		la += 0.00218166156;
		while (map[int((floor((yPos+yo)/w_tille)) * mapX + int((floor((xPos+xo)/w_tille))))] == 0) {
			float pdx = cos(la)*2;
			float pdy = sin(la)*2;

			yo += pdy;
			xo += pdx;
		}
		if (map[int((floor((yPos + yo) / w_tille)) * mapX + int((floor((xPos + xo) / w_tille))))] == 1) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			SDL_RenderDrawLine(renderer, xPos+16, yPos+16, xPos+xo, yPos+yo);
		}

		if (map[int((floor((yPos + yo) / w_tille)) * mapX + int((floor((xPos + xo) / w_tille))))] == 2) {
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderDrawLine(renderer, xPos + 16, yPos + 16, xPos + xo, yPos + yo);
		}
	}*/
}

int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH*2, HEIGHT, 0);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	bool isRunning = true;

	SDL_GetRendererOutputSize(renderer, &mapS, &mapS); // Gets the width of the screen

	int w_tille = WIDTH / mapX; // Gets the width of a tille


	SDL_Rect player_rect = { 100,100,25,25};
	int player_speed = 6;
 
	const int PLAYER_WIDTH = 8;   // Width of each frame in the texture
	const int PLAYER_HEIGHT = 8;  // Height of each frame in the texture
	const int PLAYER_FRAME_COUNT = 32; // Total number of frames in the texture
	const int FRAMES_PER_ROW = 32; // Number of frames per row in the texture

	const double ANGLE_PER_FRAME = static_cast<double>(360) / FRAMES_PER_ROW;
	const double RAD_PER_FRAME = PI * ANGLE_PER_FRAME / 180;

	int currentFrame = 0;


	static int lastTime;

	// Variables to store the player's position
	pdx = cos(pa) * 3;
	pdy = sin(pa) * 3;

	SDL_Surface* playerSurface = IMG_Load("C:\\Users\\Petr\\Desktop\\player.png");
	SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_FreeSurface(playerSurface);


	while (isRunning) {
		// Limiting the FPS
		lastFrame = SDL_GetTicks();
		if (lastFrame >= (lastFrame + 1000)) {
			lastTime = lastFrame;
			fps = frameCount;
			frameCount = 0;

		}
		frameCount++;
		int timerFPS = SDL_GetTicks() - lastFrame;
		if (timerFPS < (1000 / 60)) {
			SDL_Delay((1000 / 60)-timerFPS);
		}


		Input(isRunning, player_speed, w_tille, mapX, mapY, mapS, map, xPos, yPos, pdx, pdy, pa, PLAYER_WIDTH, PLAYER_HEIGHT, currentFrame, RAD_PER_FRAME); // Movement, QUIT

		SDL_RenderClear(renderer);

		DrawMap(renderer, mapX, mapY, mapS, map, player_rect); // Draws the map

		 // Calculate the source rectangle based on the current frame
		int srcX = round(currentFrame * PLAYER_WIDTH);
		int srcY = 0; // Since all frames are on a single row, y is always 0

		// Set up the source and destination rectangles
		SDL_Rect srcRect = { srcX, srcY, PLAYER_WIDTH, PLAYER_HEIGHT };
		SDL_Rect destRect = { xPos, yPos, PLAYER_WIDTH * 4, PLAYER_HEIGHT * 4};

		// Render the player's current frame
		SDL_RenderCopy(renderer, playerTexture, &srcRect, &destRect);

		/*SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderDrawLine(renderer, xPos + 16, yPos + 16, xPos + 16 + pdx*50, yPos + 16 + pdy*50);*/

		raycasting(mapX, mapY, mapS, map, xPos, yPos, pa, renderer, w_tille, currentFrame);

		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
