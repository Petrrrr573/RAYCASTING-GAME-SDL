#include "map_editor.h"

void MapEditor::update(int* map, int& mapX, int& mapY, int& mapSize) {
	SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEMOTION){
            SDL_GetMouseState(&mouseX, &mouseY);
        }
    }

    if (mouseX < WIDTH && mouseX >= 0) {
        if (mouseY < HEIGHT && mouseY >= 0) {
            // X, Y mouse grid possition
            int mouseGridX = mouseX / game.tilleWidth;
            int mouseGridY = mouseY / game.tilleWidth;


            const Uint8* state = SDL_GetKeyboardState(NULL);

            SDL_PollEvent(&event);

            if (state[SDL_SCANCODE_A]) {
                map[mouseGridY * mapX + mouseGridX] = 1;
            }
            if (state[SDL_SCANCODE_S]) {
                map[mouseGridY * mapX + mouseGridX] = 2;
            }
            if (state[SDL_SCANCODE_D]) {
                map[mouseGridY * mapX + mouseGridX] = 0;
            }
        }
    }
}