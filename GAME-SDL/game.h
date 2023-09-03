#pragma once

// game Class
class Game {
public:
	int mapX = 16, mapY = 16, mapSize = 256;

	int map[256] = {
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,1,1,1,2,1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,
		1,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,
		1,0,0,1,0,0,0,0,0,0,2,2,2,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	bool isRunning;
	int tilleWidth;

	SDL_Window* window;
	SDL_Renderer* renderer;

	int lastFrame, frameCount, fps;

	int timerFPS;

	void MakeWindow(const char* name);
	void DrawMap();
	void HandleFps();
	double distance(double playerX, double playerY, double rayX, double rayY, double rayAngle, double playerAngle);
	void raycasting(double xPos, double yPos, double playerAngle, int& currentFrame);
};