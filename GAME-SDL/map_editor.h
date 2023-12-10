#pragma once
#include "main.h"
#include "game.h"

class MapEditor {
	public: 
		bool isRunning;

		// X, Y mouse possition
		int mouseX = 0;
		int mouseY = 0;

		void update(int& mapX, int& mapY, int& mapSize, std::vector<int>& map, int& tilleWidth);
		void clearMap(std::vector<int>& map, int& mapSize);
		void saveMap(int& mapX, int& mapY, int& mapSize, std::vector<int>& map, int& tilleWidth, const std::string& filename);
		void openMap(int& mapX, int& mapY, int& mapSize, std::vector<int>& map, int& tilleWidth, const std::string& filename);
};