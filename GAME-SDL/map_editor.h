#pragma once
#include "main.h"
#include "game.h"

class MapEditor {
	public: 
		Game game;
		bool isRunning;

		// X, Y mouse possition
		int mouseX = 0;
		int mouseY = 0;

		void update(int* map, int& mapX, int& mapY, int& mapSize);
};