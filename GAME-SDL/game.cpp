#include <SDL.h>
#include <SDL_image.h>

#include "game.h"

// game Class
void Game::MakeWindow(const char* name, int width, int height) {
	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

	renderer = SDL_CreateRenderer(window, -1, 0);

	for (int i = 0; i < mapSize; i++) {
		map.push_back(0);
	}

	wallSurface = IMG_Load("wall.png");
	wallTexture = SDL_CreateTextureFromSurface(renderer, wallSurface);
	SDL_FreeSurface(wallSurface);
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
			SDL_Rect rect = { x * tilleWidth + 1, y * tilleWidth + 1,tilleWidth - 1, tilleWidth - 1 };
			switch (map[y * mapX + x]) {
			case 1:
				SDL_SetRenderDrawColor(renderer, WALL_COLOR_1);
				SDL_RenderFillRect(renderer, &rect);
				break;
			case 2:
				SDL_SetRenderDrawColor(renderer, WALL_COLOR_2);
				SDL_RenderFillRect(renderer, &rect);
				break;
			case 3:
				SDL_SetRenderDrawColor(renderer, 150, 100, 100, 255);
				SDL_RenderFillRect(renderer, &rect);
				break;
			case 4:
				SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
				SDL_RenderFillRect(renderer, &rect);
				break;
			}

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			// Draw the horizontal grid lines
			SDL_RenderDrawLine(renderer, 0, y * tilleWidth, mapX * tilleWidth, y * tilleWidth);

			// Draw the vertical grid lines
			SDL_RenderDrawLine(renderer, x * tilleWidth, 0, x * tilleWidth, mapY * tilleWidth);
		}
	}

	for (const auto& hit : rayHits) {
		if (hit[0] == 1) {
			SDL_SetRenderDrawColor(renderer, WALL_COLOR_1_2);
		}
		else if (hit[0] == 2) {
			SDL_SetRenderDrawColor(renderer, WALL_COLOR_2_2);
		}
		else if (hit[0] == 3) {
			SDL_SetRenderDrawColor(renderer, 170, 0, 120, 255);
		}
		else if (hit[0] == 4) {
			SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
		}
		else {
			SDL_SetRenderDrawColor(renderer, WALL_COLOR_3);
		}
		//SDL_RenderDrawLine(renderer, hit[3] / this->tilleWidth * minimapTilleWidth, hit[4] / this->tilleWidth * minimapTilleWidth, hit[1] / this->tilleWidth * minimapTilleWidth, hit[2] / this->tilleWidth * minimapTilleWidth);
		/*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect rect = { hit[1] / this->tilleWidth * minimapTilleWidth,hit[2] / this->tilleWidth * minimapTilleWidth,2, 2 };
		SDL_RenderFillRect(renderer, &rect);*/
	}
	rayHits.clear();

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

float Game::radToDeg(float degrees) {
	return degrees * 180 / PI;
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
	stripes.clear();
	double rayY = -1, rayX = -1;

	//double horizontalY = 0, horizontalX = 0; // x, y possitions of horizontal rays
	//double verticalY = 0, verticalX = 0; // x, y possitions of vertical rays
	//double horizontalDistance = 0, verticalDistance = 0; // distances of final rays

	double xOffset = 0, yOffset = 0;

	double playerX = xPos + pWidthScaled / 2;
	double playerY = yPos + pWidthScaled / 2;

	double dx = 0, dy = 0; // x, y possitions in a tille

	int mx, my, mp; // possition on a map

	int dof; // depth of field

	double rayAngle = playerAngle;

	int FOV = 70;

	int rays = WIDTH_3D; // number of rays

	rayAngle -= degToRad(FOV / 2);

	int playerPlaneDistance = WIDTH_3D / 2 / tan(0.610865); // midle of the screen / tan(35deg)

	short prevColumn = -1;
	short currentColumn = 0;
	short nextColumn = 0;

	int transparentWallsIndex = 0;

	for (Enemy& e : enemies) {
		e.centeredXPos = e.xPos + e.pWidthScaled / 2;
		e.centeredYPos = e.yPos + e.pWidthScaled / 2;

		e.direction = radToDeg(atan2(playerY - e.centeredYPos, playerX - e.centeredXPos)) + 180;

		e.direction -= radToDeg(playerAngle);

		if (e.direction > 180) {
			e.direction -= 360;
		}
		if (e.direction < -180) {
			e.direction += 360;
		}

		e.projectionPosition = 0.5f * tan(degToRad(e.direction)) / tan(0.5f * degToRad(FOV));

		e.columnPos = 1600 - static_cast<short>(round(WIDTH_3D * (0.5f - e.projectionPosition)));

		e.distance = distance(playerX, playerY, e.centeredXPos, e.centeredYPos, rayAngle, playerAngle);
		e.height = tilleWidth / e.distance * playerPlaneDistance;

		e.lineO = HEIGHT - e.height / 1.5;
	}

	for (int r = 0; r < rays; r++) {
		Stripe stripe(renderer, wallTexture);
		//Stripe stripe2(renderer, wallTexture);

		/* ---------------------------------------- Basic for setting the main possitions of the ray ---------------------------------------- */
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
			dof = maxDof;
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

		while (dof < maxDof) {
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
				SDL_Rect rect = { rayX / this->tilleWidth * minimapTilleWidth,rayY / this->tilleWidth * minimapTilleWidth,3,3 };
				switch (map[mp]) {
				case 1:
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
					break;
				case 3:
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
					break;
				case 0:
					SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
					break;
				}
				SDL_RenderFillRect(renderer, &rect);
				if (map[mp] == 3) {
					while (tempStripes.size() < transparentWallsIndex + 1) {
						tempStripes.push_back(Stripe(renderer, wallTexture));
					}
					tempStripes[transparentWallsIndex].horizontalY = rayY, tempStripes[transparentWallsIndex].horizontalX = rayX;
					tempStripes[transparentWallsIndex].horizontalDistance = distance(playerX, playerY, tempStripes[transparentWallsIndex].horizontalX, tempStripes[transparentWallsIndex].horizontalY, rayAngle, playerAngle);
					dof++;
					rayX += xOffset;
					rayY += yOffset;
					transparentWallsIndex++;
				}
				else {
					dof = maxDof;
					break;
				}
			}
			else {
				dof++;
				rayX += xOffset;
				rayY += yOffset;
			}
		}

		stripe.horizontalY = rayY, stripe.horizontalX = rayX;
		stripe.horizontalDistance = distance(playerX, playerY, stripe.horizontalX, stripe.horizontalY, rayAngle, playerAngle);

		/* --- Check vertical lines--- */

		dof = 0;
		double nTan = -tan(rayAngle);

		// looking straight up or down
		if (rayAngle == PI3 || rayAngle == PI2) {
			rayX = playerX; rayY = playerY;
			dof = maxDof;
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

		transparentWallsIndex = 0;

		while (dof < maxDof) {
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
				SDL_Rect rect = { rayX / this->tilleWidth * minimapTilleWidth,rayY / this->tilleWidth * minimapTilleWidth,3,3 };
				switch (map[mp]) {
				case 1:
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
					break;
				case 3:
					SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
					break;
				case 0:
					SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
					break;
				}
				SDL_RenderFillRect(renderer, &rect);
				if (map[mp] == 3) {
					while (tempStripes.size() < transparentWallsIndex + 1) {
						tempStripes.push_back(Stripe(renderer, wallTexture));
					}
					tempStripes[transparentWallsIndex].verticalY = rayY, tempStripes[transparentWallsIndex].verticalX = rayX;
					tempStripes[transparentWallsIndex].verticalDistance = distance(playerX, playerY, tempStripes[transparentWallsIndex].verticalX, tempStripes[transparentWallsIndex].verticalY, rayAngle, playerAngle);
					dof++;
					rayX += xOffset;
					rayY += yOffset;
					transparentWallsIndex++;
				}
				else {
					dof = maxDof;
					break;
				}
			}
			else {
				dof++;
				rayX += xOffset;
				rayY += yOffset;
			}
		}

		stripe.verticalY = rayY, stripe.verticalX = rayX;
		stripe.verticalDistance = distance(playerX, playerY, stripe.verticalX, stripe.verticalY, rayAngle, playerAngle);

		tempStripes.push_back(stripe);
		//tempStripes.push_back(stripe2);

		for (Stripe& stripe : tempStripes) {
			// Get shorter distance
			if (stripe.horizontalDistance > stripe.verticalDistance) {
				stripe.x = stripe.verticalX;
				stripe.y = stripe.verticalY;
				stripe.distance = stripe.verticalDistance;
				stripe.horizontalHit = false;
			}
			else if (stripe.horizontalDistance < stripe.verticalDistance) {
				stripe.x = stripe.horizontalX;
				stripe.y = stripe.horizontalY;
				stripe.distance = stripe.horizontalDistance;
				stripe.horizontalHit = true;
			}

			//draws the rays on the minimap
			if (true) {
				if (true) {
					if (stripe.horizontalHit == true) {
						if (rayAngle > PI) {
							stripe.my = int(floor((stripe.y - 1) / tilleWidth));
						}
						else {
							stripe.my = int(floor(stripe.y / tilleWidth));
						}
						stripe.mx = int(floor(stripe.x / tilleWidth));
					}
					else {
						if (rayAngle > PI2 && rayAngle < PI3) {
							stripe.mx = int(floor((stripe.x - 1) / tilleWidth));
						}
						else {
							stripe.mx = int(floor(stripe.x / tilleWidth));
						}
						stripe.my = int(floor(stripe.y / tilleWidth));
					}
					stripe.mp = stripe.my * mapX + stripe.mx;

					//// hit wall
					if (stripe.mp > 0 && stripe.mp < mapX * mapY) {
						rayHits.push_back({ float(map[stripe.mp]), float(stripe.x), float(stripe.y), float(playerX), float(playerY) });
					}


					// Calculate the ray direction relative to the player's view
					float rayDirection = FOV * (0.5f * WIDTH_3D - (float)r) / (rays - 1);
						
					// Calculate the position of the column in the projection
					float rayProjectionPosition = 0.5f * tan(degToRad(rayDirection)) / tan(0.5f * degToRad(FOV));

					// Adjust the position based on the current ray angle
					currentColumn = static_cast<short>(round(WIDTH_3D * (0.5f - rayProjectionPosition)));
					nextColumn = WIDTH_3D;
					 
					stripe.height = tilleWidth / stripe.distance * playerPlaneDistance;

					float idk = 0;
					if (currentColumn < 0) {
						idk = -currentColumn;
					}

					if (r + 1 < rays) {
						float nextRayDirection = FOV * (0.5f * WIDTH_3D - (float)(r + 1)) / (rays - 1);

						// Calculate the position of the column in the projection
						float nextRayProjectionPosition = 0.5f * tan(degToRad(nextRayDirection)) / tan(0.5f * degToRad(FOV));

						// Adjust the position based on the current ray angle
						nextColumn = static_cast<short>(round(WIDTH_3D * (0.5f - nextRayProjectionPosition)));
					}

					stripe.lineO = HEIGHT - stripe.height / 1.5;

					SDL_Event event;

					const Uint8* state = SDL_GetKeyboardState(NULL);

					SDL_PollEvent(&event);

					if (state[SDL_SCANCODE_F]) {
						stripe.lineO += 700;
					}
					if (state[SDL_SCANCODE_V]) {
						stripe.lineO -= 700;
					}

					if (prevColumn < currentColumn) {
						if (stripe.mp > 0 && stripe.mp < mapX * mapY) {
							stripe.Set(tilleWidth, map, stripe.mp, stripe.x, stripe.y, rayAngle, currentColumn, nextColumn, idk);
						}
					}
				}
			}

			stripes.push_back(stripe);
		}
		tempStripes.clear();
		rayAngle += degToRad(FOV) / rays;
	}

	for (int i = 0; i < enemies.size(); i++) {
		SDL_Event event;

		const Uint8* state = SDL_GetKeyboardState(NULL);

		SDL_PollEvent(&event);

		if (state[SDL_SCANCODE_F]) {
			enemies[i].lineO += 700;
		}
		if (state[SDL_SCANCODE_V]) {
			enemies[i].lineO -= 700;
		}

		enemies[i].srcRect = { 0, 0, 50, 50 };
		enemies[i].destRect = { int(enemies[i].columnPos - enemies[i].height / 2), int(enemies[i].lineO / 2), int(enemies[i].height), int(enemies[i].height) };
	}

	std::sort(stripes.begin(), stripes.end(), [](const Stripe& a, const Stripe& b) {
		return a.distance > b.distance;
	});

	std::sort(enemies.begin(), enemies.end(), [](const Enemy& a, const Enemy& b) {
		return a.distance > b.distance;
	});

	int enemyCount = 0;
	for (int i = 0; i < stripes.size(); i++) {
		//Drawing floor
		SDL_Rect floorRect = {stripes[i].columnPos, stripes[i].lineO / 2 + stripes[i].height - 1, stripes[i].width, HEIGHT - stripes[i].lineO / 2 + stripes[i].height + 1 };
		SDL_SetRenderDrawColor(renderer, FLOOR_COLOR);
		SDL_RenderFillRect(renderer, &floorRect);


		while (enemyCount < enemies.size() && stripes[i].distance < enemies[enemyCount].distance) {
			if (abs(enemies[enemyCount].direction) < FOV) {
				enemies[enemyCount].darknessFactor = 1.0f - std::min(1.0f, static_cast<float>(enemies[enemyCount].distance) / enemies[enemyCount].maxDistance);

				SDL_GetTextureColorMod(enemies[enemyCount].bodyTexture, &enemies[enemyCount].originalR, &enemies[enemyCount].originalG, &enemies[enemyCount].originalB);

				// Calculate color modulation based on darkness factor
				Uint8 r = static_cast<Uint8>(enemies[enemyCount].originalR * enemies[enemyCount].darknessFactor);
				Uint8 g = static_cast<Uint8>(enemies[enemyCount].originalG * enemies[enemyCount].darknessFactor);
				Uint8 b = static_cast<Uint8>(enemies[enemyCount].originalB * enemies[enemyCount].darknessFactor);

				r = static_cast<Uint8>(r * enemies[enemyCount].darknessFactor);
				g = static_cast<Uint8>(g * enemies[enemyCount].darknessFactor);
				b = static_cast<Uint8>(b * enemies[enemyCount].darknessFactor);


				SDL_SetTextureColorMod(enemies[enemyCount].bodyTexture, r, g, b);

				// Render the texture
				SDL_RenderCopy(renderer, enemies[enemyCount].bodyTexture, &enemies[enemyCount].srcRect, &enemies[enemyCount].destRect);

				// Reset the color modulation back to its original state
				SDL_SetTextureColorMod(enemies[enemyCount].bodyTexture, enemies[enemyCount].originalR, enemies[enemyCount].originalG, enemies[enemyCount].originalB);
			}
			enemyCount++;
		}
		stripes[i].Draw(renderer);
	}
	for (int e = enemyCount; e < enemies.size(); e++) {
		if (abs(enemies[e].direction) < FOV) {
			enemies[e].darknessFactor = 1.0f - std::min(1.0f, static_cast<float>(enemies[e].distance) / enemies[e].maxDistance);

			SDL_GetTextureColorMod(enemies[e].bodyTexture, &enemies[e].originalR, &enemies[e].originalG, &enemies[e].originalB);

			// Calculate color modulation based on darkness factor
			Uint8 r = static_cast<Uint8>(enemies[e].originalR * enemies[e].darknessFactor);
			Uint8 g = static_cast<Uint8>(enemies[e].originalG * enemies[e].darknessFactor);
			Uint8 b = static_cast<Uint8>(enemies[e].originalB * enemies[e].darknessFactor);

			r = static_cast<Uint8>(r * enemies[e].darknessFactor);
			g = static_cast<Uint8>(g * enemies[e].darknessFactor);
			b = static_cast<Uint8>(b * enemies[e].darknessFactor);


			SDL_SetTextureColorMod(enemies[e].bodyTexture, r, g, b);

			// Render the texture
			SDL_RenderCopy(renderer, enemies[e].bodyTexture, &enemies[e].srcRect, &enemies[e].destRect);

			// Reset the color modulation back to its original state
			SDL_SetTextureColorMod(enemies[e].bodyTexture, enemies[e].originalR, enemies[e].originalG, enemies[e].originalB);
		}
	}
}