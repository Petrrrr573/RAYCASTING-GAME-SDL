#pragma once
#include "main.h"
#include "player.h"

class Enemy {
public:
	// Variables to store the player's position
	double xPos;
	double yPos;
	double playerAngle;
	double distance;
	float maxDistance = MAXVISIBLEDISTANCE;
	Uint8 originalR, originalG, originalB;
	float darknessFactor;
	double height;
	double lineO;
	double centeredXPos, centeredYPos;

	int up, down, left, right; //numbers for keys for moving

	SDL_Rect srcRect;
	SDL_Rect destRect;

	float direction;
	float projectionPosition;
	double columnPos;

	int playerSpeed = 6;

	int playerWidth = 8;   // Width of each frame in the texture
	int playerHeight = 8;  // Height of each frame in the texture
	int playerFrameCount = 32; // Total number of frames in the texture
	int framesPerRow = 32; // Number of frames per row in the texture

	double anglePerFrame = static_cast<double>(360) / framesPerRow; // Number of degreses for each frame
	double radPerFrame = PI * anglePerFrame / 180; // Number of radiants for each frame

	float scale = 1;
	int pWidthScaled = 20;

	int currentFrame = 0;

	double pdx = cos(playerAngle) * playerSpeed;
	double pdy = sin(playerAngle) * playerSpeed;

	std::string imgName = "enemy.png";

	SDL_Surface* playerSurface;
	SDL_Texture* playerTexture;

	std::string imgBodyName = "ghost.png";

	SDL_Surface* bodySurface;
	SDL_Texture* bodyTexture;


	Enemy(int x, int y, int type) {
		xPos = x;
		yPos = y;
		switch (type) {
		case 0:
			up = 82;
			down = 81;
			left = 80;
			right = 79;
			break;
		case 1:
			up = 12;
			down = 14;
			left = 13;
			right = 15;
			break;
		}
	}

	void Update(SDL_Renderer* renderer, int tilleWidth, int mapX);
	// Handles inputs
	void Input(bool& isRunning, int tilleWidth, int mapX, int mapY, int mapSize, std::vector<int>& map);
	void Draw(SDL_Renderer* renderer, int tilleWidth);
};

