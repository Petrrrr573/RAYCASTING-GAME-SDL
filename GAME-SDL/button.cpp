#include "button.h"

void Button::Draw(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, buttonTexture, &srcRect, &destRect);
}

bool Button::Check(int mouseX, int mouseY) {
	if (mouseX > _x && mouseX < _x + _w) {
		if (mouseY > _y && mouseY < _y + _h) {
			return true;
		}
	}
}