#pragma once
#include "main.h"
#include "player.h"

class Enemy{
public:
	// Variables to store the player's position
	double xPos;
	double yPos;
	double playerAngle;

	int playerSpeed = 6;

	const int playerWidth = 8;   // Width of each frame in the texture
	const int playerHeight = 8;  // Height of each frame in the texture
	const int playerFrameCount = 32; // Total number of frames in the texture
	const int framesPerRow = 32; // Number of frames per row in the texture

	const double anglePerFrame = static_cast<double>(360) / framesPerRow; // Number of degreses for each frame
	const double radPerFrame = PI * anglePerFrame / 180; // Number of radiants for each frame

	float scale = 1;
	int pWidthScaled = 20;

	int currentFrame = 0;

	double pdx = cos(playerAngle) * playerSpeed;
	double pdy = sin(playerAngle) * playerSpeed;

	std::string imgName = "enemy.png";

	SDL_Surface* playerSurface;
	SDL_Texture* playerTexture;

	std::string imgBodyName = "enemy-body.png";

	SDL_Surface* bodySurface;
	SDL_Texture* bodyTexture;


	Enemy(int x, int y) {
		xPos = x;
		yPos = y;
	}

	void Update(SDL_Renderer* renderer, int tilleWidth, int mapX);
	// Handles inputs
	void Input(bool& isRunning, int tilleWidth, int mapX, int mapY, int mapSize, std::vector<int>& map);
	void Draw(SDL_Renderer* renderer, int tilleWidth);
};

