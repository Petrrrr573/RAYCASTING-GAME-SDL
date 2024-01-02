#include "enemy.h"

void Enemy::Update(SDL_Renderer* renderer, int tilleWidth, int mapX) {
	playerSurface = IMG_Load(imgName.c_str());
	playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_FreeSurface(playerSurface);

	scale = 16 / mapX;
	if (scale <= 0) {
		scale = 0.5;
	}
}

void Enemy::Input(bool& isRunning, int tilleWidth, int mapX, int mapY, int mapSize, std::vector<int>& map) {
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

	if (state[SDL_SCANCODE_LEFT]) {
		currentFrame += 1;
		playerAngle -= radPerFrame;
		if (playerAngle < 0) {
			playerAngle += 2 * PI;
		}
		pdx = cos(playerAngle) * playerSpeed;
		pdy = sin(playerAngle) * playerSpeed;
	}

	if (state[SDL_SCANCODE_RIGHT]) {
		currentFrame -= 1;
		playerAngle += radPerFrame;
		if (playerAngle > 2 * PI) {
			playerAngle -= 2 * PI;
		}
		pdx = cos(playerAngle) * playerSpeed;
		pdy = sin(playerAngle) * playerSpeed;
	}

	if (state[SDL_SCANCODE_UP]) {
		if (map[int((floor(yPos + pdy) / tilleWidth)) * mapX + int(floor((xPos) / tilleWidth))] == 0 && map[int((floor(yPos + pdy) / tilleWidth)) * mapX + int(floor((xPos + pWidthScaled) / tilleWidth))] == 0 && map[int((floor(yPos + pdy + pWidthScaled) / tilleWidth)) * mapX + int(floor((xPos + pWidthScaled) / tilleWidth))] == 0 && map[int((floor(yPos + pdy + pWidthScaled) / tilleWidth)) * mapX + int(floor((xPos) / tilleWidth))] == 0) {
			yPos += pdy;
		}
		if (map[int((floor(yPos) / tilleWidth)) * mapX + int(floor((xPos + pdx) / tilleWidth))] == 0 && map[int((floor(yPos) / tilleWidth)) * mapX + int(floor((xPos + pdx + pWidthScaled) / tilleWidth))] == 0 && map[int((floor(yPos + pWidthScaled) / tilleWidth)) * mapX + int(floor((xPos + pdx + pWidthScaled) / tilleWidth))] == 0 && map[int((floor(yPos + pWidthScaled) / tilleWidth)) * mapX + int(floor((xPos + pdx) / tilleWidth))] == 0) {
			xPos += pdx;
		}
	}

	if (state[SDL_SCANCODE_DOWN]) {
		if (map[int((floor(yPos - pdy) / tilleWidth)) * mapX + int(floor((xPos) / tilleWidth))] == 0 && map[int((floor(yPos - pdy) / tilleWidth)) * mapX + int(floor((xPos + pWidthScaled) / tilleWidth))] == 0 && map[int((floor(yPos - pdy + pWidthScaled) / tilleWidth)) * mapX + int(floor((xPos + pWidthScaled) / tilleWidth))] == 0 && map[int((floor(yPos - pdy + pWidthScaled) / tilleWidth)) * mapX + int(floor((xPos) / tilleWidth))] == 0) {
			yPos -= pdy;
		}
		if (map[int((floor(yPos) / tilleWidth)) * mapX + int(floor((xPos - pdx) / tilleWidth))] == 0 && map[int((floor(yPos) / tilleWidth)) * mapX + int(floor((xPos - pdx + pWidthScaled) / tilleWidth))] == 0 && map[int((floor(yPos + pWidthScaled) / tilleWidth)) * mapX + int(floor((xPos - pdx + pWidthScaled) / tilleWidth))] == 0 && map[int((floor(yPos + pWidthScaled) / tilleWidth)) * mapX + int(floor((xPos - pdx) / tilleWidth))] == 0) {
			xPos -= pdx;
		}
	}
}

void Enemy::Draw(SDL_Renderer* renderer, int tilleWidth) {

	// Calculate the source rectangle based on the current frame
	int srcX = round(currentFrame * playerWidth);
	int srcY = 0; // Since all frames are on a single row, y is always 0

	// Set up the source and destination rectangles
	SDL_Rect srcRect = { srcX, srcY, playerWidth, playerHeight };
	SDL_Rect destRect = { xPos / 50 * tilleWidth, yPos / 50 * tilleWidth, 10 * scale, 10 * scale };

	// Render the player's current frame
	SDL_RenderCopy(renderer, playerTexture, &srcRect, &destRect);
}