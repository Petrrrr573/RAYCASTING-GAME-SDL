#pragma once
#include "main.h"

class Button {
public:
	int _x;
	int _y;
	int _w;
	int _h;

	SDL_Surface* buttonSurface;
	SDL_Texture* buttonTexture;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	Button(SDL_Renderer* renderer, int x, int y, int type) {
		buttonSurface = IMG_Load("C:\\Users\\Petr\\Desktop\\loadsavebuttons.png");
		buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
		SDL_FreeSurface(buttonSurface);

		if (type == 1 || type == 2){
			_w = 150;
			_h = 100;
		}
		_x = x;
		_y = y;
		srcRect = { (type - 1) * 150, 0, _w, _h };
		destRect = { x, y, _w, _h };
	}

	void Draw(SDL_Renderer* renderer);

	bool Check(int mouseX, int mouseY);
};