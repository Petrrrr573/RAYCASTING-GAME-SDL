#pragma once
#include "main.h"

class Stripe {
public:
	SDL_Rect wSrcRect;
	SDL_Rect wDestRect;

	double distance;
	double horizontalY, horizontalX; // x, y possitions of horizontal rays
	double verticalY, verticalX; // x, y possitions of vertical rays
	double horizontalDistance = INFINITY, verticalDistance = INFINITY; // distances of final rays
	double y = -1, x = -1;
	int mx, my, mp; // possition on a map
	float maxDistance = MAXVISIBLEDISTANCE;
	Uint8 originalR, originalG, originalB;
	float darknessFactor;
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
