#pragma once
#include "main.h"
#include "game.h"
#include "button.h"

class MapEditor {
	public: 
		bool isRunning;

		Button saveButton;
		Button loadButton;

		Button wall1;
		Button wall2;
		Button wall3;

		MapEditor(SDL_Renderer* renderer);

		// X, Y mouse possition
		int mouseX = 0;
		int mouseY = 0;

		// X, Y mouse grid possition
		int mouseGridX;
		int mouseGridY;

		int currentWall;

		void update(int& mapX, int& mapY, int& mapSize, std::vector<int>& map, int& tilleWidth, SDL_Renderer* renderer);
		void clearMap(std::vector<int>& map, int& mapSize);
		void saveMap(int& mapX, int& mapY, int& mapSize, std::vector<int>& map, int& tilleWidth, const std::string& filename);
		void openMap(int& mapX, int& mapY, int& mapSize, std::vector<int>& map, int& tilleWidth, const std::string& filename);
};