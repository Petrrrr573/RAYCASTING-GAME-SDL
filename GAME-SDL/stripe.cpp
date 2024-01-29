#include "stripe.h"


void Stripe::Set(int tilleWidth, std::vector<int>& map, int mp, int rayX, int rayY, float rayAngle, int currentColumn, int nextColumn, int idk) {
	int srcX;
	int srcY = 0;
	wallX = rayX - floor(rayX / tilleWidth) * tilleWidth;
	wallY = rayY - floor(rayY / tilleWidth) * tilleWidth;
	width = nextColumn - currentColumn;
	columnPos = currentColumn + idk;
	if (horizontalHit) {
		srcX = floor(wallX);
		if (rayAngle < PI) {
			srcX = 49 - srcX;
		}
	}
	else {
		srcX = floor(wallY);;
		if (rayAngle > PI2 && rayAngle < PI3) {
			srcX = 49 - srcX;
		}
	}

	srcX += (map[mp] - 1) * 50;
	wSrcRect = { srcX, srcY, 1, 50 };
	wDestRect = { int(columnPos), int(lineO / 2), int(width), int(height) };
}

void Stripe::Draw(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, wallTexture, &wSrcRect, &wDestRect);
}