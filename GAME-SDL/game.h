#pragma once
#include "main.h"
#include "stripe.h"
#include "enemy.h"

// game Class
class Game {
public:
	int mapX = 16, mapY = 16, mapSize = mapX * mapY;

	std::vector<int> map;
	std::vector<std::vector<float>> rayHits;

	std::vector<Stripe> stripes;
	std::vector<Stripe> tempStripes;
	std::vector<Enemy> enemies;

	int minimapWidth = 320;

	bool isRunning;
	int tilleWidth = 50;
	int mapEditorTilleWidth = HEIGHT / mapX;
	int minimapTilleWidth = minimapWidth / mapX;

	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_Surface* wallSurface;
	SDL_Texture* wallTexture;

	int lastFrame, frameCount, fps;

	int timerFPS;

	double lineO;

	int maxDof = 20;

	void MakeWindow(const char* name, int width, int height);
	void DrawMap(int tilleWidth);
	void HandleFps();
	void Input(bool& running);
	float degToRad(float degrees);
	float radToDeg(float degrees);
	double distance(double playerX, double playerY, double rayX, double rayY, double rayAngle, double playerAngle);
	void raycasting(double xPos, double yPos, double playerAngle, int& currentFrame, float pWidthScaled);
};