#include "map_editor.h"

void MapEditor::update(int& mapX, int& mapY, int& mapSize, std::vector<int>& map, int& tilleWidth) {
	SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEMOTION){
            SDL_GetMouseState(&mouseX, &mouseY);
        }
    }

    if (mouseX < WIDTH && mouseX >= 0) {
        if (mouseY < HEIGHT && mouseY >= 0) {
            // X, Y mouse grid possition
            int mouseGridX = mouseX / tilleWidth;
            int mouseGridY = mouseY / tilleWidth;


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

            if (state[SDL_SCANCODE_I]) {
                mapX++;
                mapY++;
                mapSize = mapX * mapY;
                tilleWidth = WIDTH / mapX;
                map.clear();
                for (int i = 0; i < mapSize; i++) {
                    map.push_back(0);
                }
            }
            if (state[SDL_SCANCODE_O]) {
                mapX--;
                mapY--;
                mapSize = mapX * mapY;
                tilleWidth = WIDTH / mapX;
                map.clear();
                for (int i = 0; i < mapSize; i++) {
                    map.push_back(0);
                }
            }

            if (state[SDL_SCANCODE_M]) {
                saveMap(mapX, mapY, mapSize, map, tilleWidth, "map.dat");
                mapSize = mapX * mapY;
                tilleWidth = WIDTH / mapX;
            }
            if (state[SDL_SCANCODE_N]) {
                openMap(mapX, mapY, mapSize, map, tilleWidth, "map.dat");
                mapSize = mapX * mapY;
                tilleWidth = WIDTH / mapX;
            }
        }
    }
}

void MapEditor::saveMap(int& mapX, int& mapY, int& mapSize, std::vector<int>& map, int& tilleWidth, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    if (file.is_open()) {
        // Write the map dimensions
        file.write(reinterpret_cast<char*>(&mapX), sizeof(int));
        file.write(reinterpret_cast<char*>(&mapY), sizeof(int));

        // Write the map data
        file.write(reinterpret_cast<char*>(map.data()), mapSize * sizeof(int));

        file.close();
        std::cout << "Map saved successfully.\n";
    }
    else {
        std::cerr << "Error opening file for writing.\n";
    }
}

void MapEditor::openMap(int& mapX, int& mapY, int& mapSize, std::vector<int>& map, int& tilleWidth, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (file.is_open()) {
        // Read the map dimensions
        file.read(reinterpret_cast<char*>(&mapX), sizeof(int));
        file.read(reinterpret_cast<char*>(&mapY), sizeof(int));

        // Resize the map vector
        map.resize(mapSize);

        // Read the map data
        file.read(reinterpret_cast<char*>(map.data()), mapSize * sizeof(int));

        file.close();
        std::cout << "Map loaded successfully.\n";
    }
    else {
        std::cerr << "Error opening file for reading.\n";
    }
}