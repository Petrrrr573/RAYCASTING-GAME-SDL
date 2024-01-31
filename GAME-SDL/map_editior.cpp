#include "map_editor.h"

MapEditor::MapEditor(SDL_Renderer* renderer)
    : saveButton(renderer, 900, 100, 1), 
    loadButton(renderer, 1100, 100, 2), 
    wall1(renderer, 900, 300, 3), 
    wall2(renderer, 900, 375, 4), 
    wall3(renderer, 900, 450, 5),
    wall4(renderer, 900, 525, 6) {

    buttons.push_back(saveButton);
    buttons.push_back(loadButton);
    buttons.push_back(wall1);
    buttons.push_back(wall2);
    buttons.push_back(wall3);
    buttons.push_back(wall4);
}

void MapEditor::update(int& mapX, int& mapY, int& mapSize, std::vector<int>& map, int& tilleWidth, SDL_Renderer* renderer) {
    for (Button& button : buttons) {
        button.Draw(renderer);
   }

    SDL_Event event;

    SDL_GetMouseState(&mouseX, &mouseY);

    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (mouseX < tilleWidth*mapX && mouseX >= 0) {
        if (mouseY < tilleWidth*mapY && mouseY >= 0) {
            mouseGridX = mouseX / tilleWidth;
            mouseGridY = mouseY / tilleWidth;

            if (state[SDL_SCANCODE_A]) {
                map[mouseGridY * mapX + mouseGridX] = 1;
            }
            if (state[SDL_SCANCODE_S]) {
                map[mouseGridY * mapX + mouseGridX] = 2;
            }
            if (state[SDL_SCANCODE_D]) {
                map[mouseGridY * mapX + mouseGridX] = 3;
            }
            if (state[SDL_SCANCODE_BACKSPACE]) {
                map[mouseGridY * mapX + mouseGridX] = 0;
            }

            // Clears the map
            if (state[SDL_SCANCODE_C]) {
                clearMap(map, mapSize);
            }

            if (state[SDL_SCANCODE_RETURN]) {
                map[mouseGridY * mapX + mouseGridX] = currentWall;
            }
        }
    }
    else {
        if (state[SDL_SCANCODE_RETURN]) {
            if (saveButton.Check(mouseX, mouseY) == true) {
                saveMap(mapX, mapY, mapSize, map, tilleWidth, "map.dat");
                mapSize = mapX * mapY;
                tilleWidth = HEIGHT / mapX;
            }

            if (loadButton.Check(mouseX, mouseY) == true) {
                openMap(mapX, mapY, mapSize, map, tilleWidth, "map.dat");
                mapSize = mapX * mapY;
                tilleWidth = HEIGHT / mapX;
            }

            if (wall1.Check(mouseX, mouseY) == true) {
                currentWall = 1;
            }
            if (wall2.Check(mouseX, mouseY) == true) {
                currentWall = 2;
            }
            if (wall3.Check(mouseX, mouseY) == true) {
                currentWall = 3;
            }
            if (wall4.Check(mouseX, mouseY) == true) {
                currentWall = 4;
            }
        }

        // Makes the map bigger
        if (state[SDL_SCANCODE_I]) {
            mapX++;
            mapY++;
            mapSize = mapX * mapY;
            tilleWidth = HEIGHT / mapX;
            clearMap(map, mapSize);
        }
        // Makes the map smaller
        if (state[SDL_SCANCODE_O]) {
            mapX--;
            mapY--;
            mapSize = mapX * mapY;
            tilleWidth = HEIGHT / mapX;
            clearMap(map, mapSize);
        }

        // Saves the map
        if (state[SDL_SCANCODE_M]) {
            saveMap(mapX, mapY, mapSize, map, tilleWidth, "map.dat");
            mapSize = mapX * mapY;
            tilleWidth = HEIGHT / mapX;
        }
        // Loads the saved map
        if (state[SDL_SCANCODE_N]) {
            openMap(mapX, mapY, mapSize, map, tilleWidth, "map.dat");
            mapSize = mapX * mapY;
            tilleWidth = HEIGHT / mapX;
        }
    }
}

void MapEditor::clearMap(std::vector<int>& map, int& mapSize) {
    map.clear();
    for (int i = 0; i < mapSize; i++) {
        map.push_back(0);
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