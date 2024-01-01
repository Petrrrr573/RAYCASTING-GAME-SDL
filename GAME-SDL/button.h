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

		switch (type){
		case 1: case 2:
			_w = 150;
			_h = 100;
			srcRect = { (type - 1) * _w, 0, _w, _h };
			break;
		case 3: case 4: case 5:
			_w = 50;
			_h = 50;
			srcRect = { (type - 3) * _w, 100, _w, _h };
			break;
		}
		_x = x;
		_y = y;
		destRect = { x, y, _w, _h };
	}

	void Draw(SDL_Renderer* renderer);

	bool Check(int mouseX, int mouseY);
};