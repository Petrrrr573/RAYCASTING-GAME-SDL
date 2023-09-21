#pragma once
#include "main.h"

// game Class
class Game {
public:
	int mapX=16, mapY=16, mapSize = mapX*mapY;

	std::vector<int> map;

	bool isRunning;
	int tilleWidth = WIDTH / mapX; // Gets the width of a tille

	SDL_Window* window;
	SDL_Renderer* renderer;

	int lastFrame, frameCount, fps;

	int timerFPS;

	void MakeWindow(const char* name, int width, int height, bool& running);
	void DrawMap();
	void HandleFps();
	void Input(bool&running);
	double distance(double playerX, double playerY, double rayX, double rayY, double rayAngle, double playerAngle);
	void raycasting(double xPos, double yPos, double playerAngle, int& currentFrame);
};