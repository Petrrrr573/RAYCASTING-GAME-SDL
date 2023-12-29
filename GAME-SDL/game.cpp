#include <SDL.h>
#include <SDL_image.h>

#include "game.h"

// game Class
void Game::MakeWindow(const char* name, int width, int height, bool& running) {
	running = true;

	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

	renderer = SDL_CreateRenderer(window, -1, 0);

	for (int i = 0; i < mapSize; i++) {
		map.push_back(0);
	}
}

// Draws the minimap
void Game::DrawMap(int tilleWidth) {
	int x, y;

	SDL_SetRenderDrawColor(renderer, SKY_COLOR);
	SDL_Rect rect = {0,0, mapX * tilleWidth, mapY * tilleWidth};
	SDL_RenderFillRect(renderer, &rect);

	// Draw the map tiles
	for (y = 0; y < mapY; y++) {
		for (x = 0; x < mapX; x++) {
			if (map[y * mapX + x] == 1) {
				SDL_SetRenderDrawColor(renderer, WALL_COLOR_1);
				SDL_Rect rect = { x * tilleWidth + 1, y * tilleWidth + 1,tilleWidth - 1, tilleWidth - 1 };
				SDL_RenderFillRect(renderer, &rect);
			}

			if (map[y * mapX + x] == 2) {
				SDL_SetRenderDrawColor(renderer, WALL_COLOR_2);
				SDL_Rect rect = { x * tilleWidth + 1, y * tilleWidth + 1,tilleWidth - 1, tilleWidth - 1 };
				SDL_RenderFillRect(renderer, &rect);
			}

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			// Draw the horizontal grid lines
			SDL_RenderDrawLine(renderer, 0, y * tilleWidth, mapX * tilleWidth, y * tilleWidth);

			// Draw the vertical grid lines
			SDL_RenderDrawLine(renderer, x * tilleWidth, 0, x * tilleWidth, mapY * tilleWidth);
		}
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


void Game::Input(bool& running) {

	SDL_Event event;

	const Uint8* state = SDL_GetKeyboardState(NULL);

	SDL_PollEvent(&event);

	if (state[SDL_SCANCODE_ESCAPE]) {
		running = false;
	}
	if (event.type == SDL_QUIT) {
		running = false;
	}
}

float Game::degToRad(float degrees) {
	return degrees * PI / 180;
}

// Gets the distance of a ray
double Game::distance(double playerX, double playerY, double rayX, double rayY, double rayAngle, double playerAngle) {
	double distance = 0;

	double xDifference = rayX - playerX;
	double yDifference = rayY - playerY;

	//distance = xDifference * cos(playerAngle) + yDifference * sin(playerAngle);
	distance = sqrt(xDifference * xDifference + yDifference * yDifference);

	distance = distance * cos(rayAngle - playerAngle);

	return distance;
}

void Game::raycasting(double xPos, double yPos, double playerAngle, int& currentFrame, float pWidthScaled) {
	double rayY = -1, rayX = -1;

	double horizontalY = 0, horizontalX = 0; // x, y possitions of horizontal rays
	double verticalY = 0, verticalX = 0; // x, y possitions of vertical rays
	double horizontalDistance = 0, verticalDistance = 0; // distances of final rays

	double finalDistance = 0; // Final distance

	double xOffset = 0, yOffset = 0;

	double playerX = xPos + pWidthScaled / 2;
	double playerY = yPos + pWidthScaled / 2;

	double dx = 0, dy = 0; // x, y possitions in a tille

	int mx, my, mp; // possition on a map

	int dof; // depth of field

	double rayAngle = playerAngle;

	int FOV = 70;

	int rays = WIDTH_3D; // number of rays

	rayAngle -= degToRad(FOV/2);

	int playerPlaneDistance = WIDTH_3D/2 / tan(0.610865); // midle of the screen / tan(35deg)

	short prevColumn = 0;
	short currentColumn = 0;
	short nextColumn = 0;

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
			rayY = floor(playerY / tilleWidth) * tilleWidth;
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
			if (rayAngle > PI) {
				my = int(floor((rayY - 1) / tilleWidth));
			}
			else {
				my = int(floor(rayY / tilleWidth));
			}
			mx = int(floor(rayX / tilleWidth));
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
			rayX = floor(playerX / tilleWidth) * tilleWidth;
			dx = playerX - rayX;
			rayY = (dx)*nTan + playerY;
			xOffset = -tilleWidth;
			yOffset = -xOffset * nTan;
		}

		while (dof < 30) {
			if (rayAngle > PI2 && rayAngle < PI3) {
				mx = int(floor((rayX - 1) / tilleWidth));
			}
			else {
				mx = int(floor(rayX / tilleWidth));
			}
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

		//draws the rays on the minimap
		if (rayX >= 0 && rayX <= WIDTH/2) {
			if (rayY >= 0 && rayY <= HEIGHT) {
				if (horizontalHit == true) {
					if (rayAngle > PI) {
						my = int(floor((rayY - 1) / tilleWidth));
					}
					else {
						my = int(floor(rayY / tilleWidth));
					}
					mx = int(floor(rayX / tilleWidth));
				}
				else {
					if (rayAngle > PI2 && rayAngle < PI3) {
						mx = int(floor((rayX - 1) / tilleWidth));
					}
					else {
						mx = int(floor(rayX / tilleWidth));
					}
					my = int(floor(rayY / tilleWidth));
				}
				mp = my * mapX + mx;

				// hit wall
				if (mp > 0 && mp < mapX * mapY) {
					if (map[mp] == 1) {
						SDL_SetRenderDrawColor(renderer, WALL_COLOR_1_2);
					}
					else if (map[mp] == 2) {
						SDL_SetRenderDrawColor(renderer, WALL_COLOR_2_2);
					}
					else {
						SDL_SetRenderDrawColor(renderer, WALL_COLOR_3);
					}
					SDL_RenderDrawLine(renderer, playerX/tilleWidth*minimapTilleWidth, playerY / tilleWidth * minimapTilleWidth, rayX / tilleWidth * minimapTilleWidth, rayY / tilleWidth * minimapTilleWidth);
				}

				// Calculate the ray direction relative to the player's view
				float rayDirection = FOV * (0.5f * WIDTH_3D - (float)r) / (rays-1);

				// Calculate the position of the column in the projection
				float rayProjectionPosition = 0.5f * tan(degToRad(rayDirection)) / tan(0.5f * degToRad(FOV));

				// Adjust the position based on the current ray angle
				currentColumn = static_cast<short>(round(WIDTH_3D * (0.5f - rayProjectionPosition)));
				nextColumn = WIDTH_3D;

				double wh = tilleWidth / finalDistance * playerPlaneDistance;

				float idk = 0;
				if (currentColumn < 0) {
					idk = -currentColumn;
				}

				if (r + 1  < rays) {
					float nextRayDirection = FOV * (0.5f * WIDTH_3D - (float)(r + 1)) / (rays-1);

					// Calculate the position of the column in the projection
					float nextRayProjectionPosition = 0.5f * tan(degToRad(nextRayDirection)) / tan(0.5f * degToRad(FOV));

					// Adjust the position based on the current ray angle
					nextColumn = static_cast<short>(round(WIDTH_3D * (0.5f - nextRayProjectionPosition)));
				}

				lineO = HEIGHT - wh / 1.5;

				SDL_Event event;

				const Uint8* state = SDL_GetKeyboardState(NULL);

				SDL_PollEvent(&event);

				if (state[SDL_SCANCODE_F]) {
					lineO += 200;
				}
				if (state[SDL_SCANCODE_V]) {
					lineO -= 200;
				}

				if (prevColumn < currentColumn) {
					SDL_Rect rect = { currentColumn + idk, lineO / 2, nextColumn - currentColumn, wh };
					SDL_Rect floorRect = { currentColumn + idk, lineO / 2 + wh - 1, nextColumn - currentColumn, HEIGHT - lineO / 2 + wh + 1 };
					if (mp > 0 && mp < mapX * mapY) {
						if (map[mp] == 1) {
							if (horizontalHit == true) {
								SDL_SetRenderDrawColor(renderer, WALL_COLOR_1);
							}
							else if (horizontalHit == false) {
								SDL_SetRenderDrawColor(renderer, WALL_COLOR_1_2);
							}
						}
						else if (map[mp] == 2) {
							if (horizontalHit == true) {
								SDL_SetRenderDrawColor(renderer, WALL_COLOR_2);
							}
							else if (horizontalHit == false) {
								SDL_SetRenderDrawColor(renderer, WALL_COLOR_2_2);
							}
						}
						else if (map[mp] == 3) {
							SDL_SetRenderDrawColor(renderer, WALL_COLOR_3);
						}
						//Drawing floor
						SDL_RenderFillRect(renderer, &rect);
						SDL_SetRenderDrawColor(renderer, FLOOR_COLOR);
						SDL_RenderFillRect(renderer, &floorRect);
					}
				}
			}
		}
		rayAngle += degToRad(FOV) / rays;
	}
}