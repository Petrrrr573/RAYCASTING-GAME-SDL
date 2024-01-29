#pragma once
#include "main.h"

class Stripe {
public:
	SDL_Rect wSrcRect;
	SDL_Rect wDestRect;

	double distance;
	int lineO;
	double height;
	double width;
	float columnPos;
	bool horizontalHit = true;

	int wallX, wallY;

	SDL_Renderer* renderer;
	SDL_Texture* wallTexture;


	Stripe(SDL_Renderer* renderer, SDL_Texture* texture) {
		this->renderer = renderer;
		this->wallTexture = texture;
	}

	void Set(int tilleWidth, std::vector<int>& map, int mp, int rayX, int rayY, float rayAngle, int currentColumn, int nextColumn, int idk);
	void Draw(SDL_Renderer* renderer);
};
