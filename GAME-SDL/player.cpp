#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Player {
public:
	void handle_rotation(int& xPos, int& yPos, int& currentFrame, float ANGLE_PER_FRAME) {
		std::cout << currentFrame << std::endl;
		int player_direction = currentFrame * ANGLE_PER_FRAME;
		switch (currentFrame) {
		case 0:
			xPos += 8 / 2;
			break;
		case 8:
			yPos -= 8 / 2;
			break;
		case 16:
			xPos -= 8 / 2;
			break;
		case 24:
			yPos += 8 / 2;
		case 32:
			xPos += 8 / 2;
			break;
		}


		if (0 < player_direction && player_direction < 90 && currentFrame != 8) {

			float rad = player_direction * 3.14159 / 180;

			float tan_w = tan(rad);

			float y_long = tan_w * 8;

			xPos += 8 / 2;
			yPos -= y_long / 2;
		}

		if (90 < player_direction && player_direction < 180 && currentFrame != 16 && currentFrame != 8) {

			float rad = (180 - player_direction) * 3.14159 / 180;


			float tan_w = tan(rad);

			float y_long = tan_w * 8;

			xPos -= 8 / 2;
			yPos -= y_long / 2;
		}

		if (180 < player_direction && player_direction < 270 && currentFrame != 24 && currentFrame != 16) {
			float rad = player_direction * 3.14159 / 180;

			float tan_w = tan(rad);

			float y_long = tan_w * 8;

			xPos -= 8 / 2;
			yPos += y_long / 2;
		}

		if (270 < player_direction && player_direction < 360 && currentFrame != 24 && currentFrame != 16) {
			float rad = player_direction * 3.14159 / 180;

			float tan_w = tan(rad);

			float y_long = tan_w * 8;


			xPos += 8 / 2;
			yPos -= y_long / 2;
		}

	}

	void input(bool& isRunning, int w_tille, int mapX, int mapY, int mapS, int* map) {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					isRunning = false;

				case SDLK_a:
					if (map[int((floor(yPos) / w_tille)) * mapX + int(floor((xPos - player_speed) / w_tille))] == 0) {
						if (map[int((floor(yPos + PLAYER_WIDTH * 4) / w_tille)) * mapX + int(floor((xPos - player_speed) / w_tille))] == 0) {
							//xPos -= player_speed;

							player_direction += player_rotation_speed;
							currentFrame = 0;
							currentFrame += round(player_direction / ANGLE_PER_FRAME);
							currentFrame = fmod(32 + fmod(currentFrame, 32), 32);
						}
					}
					break;

				case SDLK_d:
					if (map[int((floor(yPos + PLAYER_WIDTH * 4) / w_tille)) * mapX + int(floor((xPos + PLAYER_WIDTH * 4 + player_speed) / w_tille))] == 0) {
						if (map[int((floor(yPos) / w_tille)) * mapX + int(floor((xPos + PLAYER_WIDTH * 4 + player_speed) / w_tille))] == 0) {
							//xPos += player_speed;
							player_direction -= player_rotation_speed;
							currentFrame = 0;
							currentFrame += round(player_direction / ANGLE_PER_FRAME);
							currentFrame = fmod(32 + fmod(currentFrame, 32), 32);
						}
					}
					break;

				case SDLK_w:
					if (map[int((floor(yPos - player_speed - 1) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0) {
						if (map[int((floor(yPos - player_speed - 1) / w_tille)) * mapX + int(floor((xPos + PLAYER_WIDTH * 4) / w_tille))] == 0) {
							handle_rotation(xPos, yPos, currentFrame, ANGLE_PER_FRAME);
						}
					}
					break;

				case SDLK_s:
					if (map[int((floor(yPos + PLAYER_WIDTH * 4 + player_speed) / w_tille)) * mapX + int(floor((xPos + PLAYER_WIDTH * 4) / w_tille))] == 0) {
						if (map[int((floor(yPos + PLAYER_WIDTH * 4 + player_speed) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0) {
							yPos += player_speed;
						}
					}
					break;
				}
				break;
			}
		}

		player_direction = fmod(360 + fmod(player_direction, 360), 360);

	}

	void load(SDL_Renderer* renderer) {
		SDL_Surface* playerSurface = IMG_Load("C:\\Users\\Petr\\Downloads\\player.png");
		SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
		SDL_FreeSurface(playerSurface);
	}

	void draw(SDL_Renderer* renderer) {
		int srcX = round(currentFrame * PLAYER_WIDTH);
		int srcY = 0; // Since all frames are on a single row, y is always 0

		// Set up the source and destination rectangles
		SDL_Rect srcRect = { srcX, srcY, PLAYER_WIDTH, PLAYER_HEIGHT };
		SDL_Rect destRect = { xPos, yPos, PLAYER_WIDTH * 4, PLAYER_HEIGHT * 4 };

		// Render the player's current frame
		SDL_RenderCopy(renderer, playerTexture, &srcRect, &destRect);
	}


private:
	int player_speed = 6;
	int player_direction = 0;
	int player_rotation_speed = 2;

	const int PLAYER_WIDTH = 8;   // Width of each frame in the texture
	const int PLAYER_HEIGHT = 8;  // Height of each frame in the texture
	const int PLAYER_FRAME_COUNT = 32; // Total number of frames in the texture
	const int FRAMES_PER_ROW = 32; // Number of frames per row in the texture

	const float ANGLE_PER_FRAME = 360 / FRAMES_PER_ROW;

	int currentFrame = 0;

	// Variables to store the player's position
	int xPos = 100;
	int yPos = 100;

	SDL_Surface* playerSurface;
	SDL_Texture* playerTexture;
};