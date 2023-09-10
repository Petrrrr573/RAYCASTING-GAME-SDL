#include <SDL.h>
#include <SDL_image.h>

#include "game.h"
#include "main.h"

// game Class
void Game::MakeWindow(const char* name) {
	isRunning = true;

	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,WIDTH * 2, HEIGHT, 0);

	renderer = SDL_CreateRenderer(window, -1, 0);


	SDL_GetRendererOutputSize(renderer, &mapSize, &mapSize); // Gets the width of thescreen

	tilleWidth = WIDTH / mapX; // Gets the width of a tille
}

// Draws the minimap
void Game::DrawMap() {
	int x, y;

	// Draw the map tiles
	for (y = 0; y < mapY; y++) {
		for (x = 0; x < mapX; x++) {
			if (map[y * mapX + x] == 1) {
				SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
				SDL_Rect rect = { x * (mapSize / mapX) + 1, y * (mapSize / mapY) + 1,mapSize / mapX - 1, mapSize / mapY - 1 };
				SDL_RenderFillRect(renderer, &rect);
			}

			if (map[y * mapX + x] == 2) {
				SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
				SDL_Rect rect = { x * (mapSize / mapX) + 1, y * (mapSize / mapY) + 1,mapSize / mapX - 1, mapSize / mapY - 1 };
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	// Draw the horizontal grid lines
	for (y = 0; y < mapY; y++) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, 0, y * (mapSize / mapY), mapSize, y * (mapSize /mapY));
	}

	// Draw the vertical grid lines
	for (x = 0; x < mapX; x++) {
		SDL_RenderDrawLine(renderer, x * (mapSize / mapX), 0, x * (mapSize / mapX),mapSize);
	}

}

// Limiting the FPS
void Game::HandleFps() {
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
double Game::distance(double playerX, double playerY, double rayX, double rayY, double rayAngle, double playerAngle) {
	double distance = 0;

	double xDifference = rayX - playerX;
	double yDifference = rayY - playerY;

	//distance = xDifference * cos(playerAngle) + yDifference * sin(playerAngle);
	distance = sqrt(xDifference*xDifference + yDifference*yDifference);

	distance = distance * cos(rayAngle - playerAngle);

	return distance;
}

void Game::raycasting(double xPos, double yPos, double playerAngle, int& currentFrame) {
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

	int FOV = 60;

	int rays = 800; // number of rays

	rayAngle -= FOV*PI/180/2;

	int playerPlaneDistance = 400 / tan(0.523599); // midle of the screen / tan(30deg)


	//Drawing floor
	SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
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
		horizontalDistance = distance(playerX, playerY, horizontalX, horizontalY,rayAngle, playerAngle);

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
		verticalDistance = distance(playerX, playerY, verticalX, verticalY, rayAngle,playerAngle);

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
				//double wh = (tilleWidth * HEIGHT) / finalDistance;
				double wh = tilleWidth/finalDistance*playerPlaneDistance;
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
					//SDL_RenderDrawLine(renderer, r* (800 / rays) + WIDTH, lineO / 2, r* (800 / rays) + WIDTH, wh+lineO / 2);
				}
			}
		}

		rayAngle += FOV*PI/180/rays;
	}
}