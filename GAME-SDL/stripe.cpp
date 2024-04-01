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

	if (map[mp] == 4) {
		srcY = 49;
		srcX += 0;
	}
	else {
		srcX += (map[mp] - 1) * 50;
	}
	wSrcRect = { srcX, srcY, 1, 50 };
	wDestRect = { int(columnPos), int(lineO / 2), int(width), int(height) };
	}

void Stripe::Draw(SDL_Renderer* renderer) {
	darknessFactor = 1.0f - std::min(1.0f, static_cast<float>(distance) / maxDistance);

	SDL_GetTextureColorMod(wallTexture, &originalR, &originalG, &originalB);

	 // Calculate color modulation based on darkness factor
	Uint8 r = static_cast<Uint8>(originalR * darknessFactor);
	Uint8 g = static_cast<Uint8>(originalG * darknessFactor);
	Uint8 b = static_cast<Uint8>(originalB * darknessFactor);

	r = static_cast<Uint8>(r * darknessFactor);
	g = static_cast<Uint8>(g * darknessFactor);
	b = static_cast<Uint8>(b * darknessFactor);


	SDL_SetTextureColorMod(wallTexture, r, g, b);

	// Render the texture
	SDL_RenderCopy(renderer, wallTexture, &wSrcRect, &wDestRect);

	// Reset the color modulation back to its original state
	SDL_SetTextureColorMod(wallTexture, originalR, originalG, originalB);
}