#include <SDL.h>
#include <SDL_image.h>
#include <iostream> 

#define WIDTH 800
#define HEIGHT 800

#define PI 3.1415926535


int mapX = 8, mapY = 8, mapS = 64;

int map[] = {
	1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,1,0,0,1,1,
	1,0,0,0,0,0,1,1,
	1,0,0,0,0,0,1,1,
	1,0,0,0,0,0,1,1,
};

double xPos = 100;
double yPos = 100;
double pdx, pdy, pa;

int lastFrame, frameCount, fps;

void DrawMap(SDL_Renderer* renderer, int mapX, int mapY, int mapS, int* map, SDL_Rect player_rect) {
	int x, y;

	// Draw the map tiles
	for (y = 0; y < mapY; y++) {
		for (x = 0; x < mapX; x++) {
			if (map[y * mapX + x] == 1) {
				SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
				SDL_Rect rect = { x * (mapS / mapX) + 1, y * (mapS / mapY) + 1, mapS / mapX - 1, mapS / mapY - 1 };
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	// Draw the horizontal grid lines
	for (y = 0; y < mapY; y++) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, 0, y * (mapS / mapY), mapS, y * (mapS / mapY));
	}

	// Draw the vertical grid lines
	for (x = 0; x < mapX; x++) {
		SDL_RenderDrawLine(renderer, x * (mapS / mapX), 0, x * (mapS / mapX), mapS);
	}


	/*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &player_rect);*/
	//SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
}

void Input(bool& isRunning, int player_speed, int w_tille, int mapX, int mapY, int mapS, int* map, double& xPos, double& yPos, double& pdx, double& pdy, double& pa, int PLAYER_WIDTH, int PLAYER_HEIGHT, int& currentFrame, double RAD_PER_FRAME) {
	if (currentFrame >= 32) { currentFrame = 0; }
	else if (currentFrame < 0) { currentFrame = 31; }

	SDL_Event event;

	const Uint8* state = SDL_GetKeyboardState(NULL);

	SDL_PollEvent(&event);

	if (state[SDL_SCANCODE_ESCAPE]) {
		isRunning = false;
	}

	if (state[SDL_SCANCODE_A]) {
		currentFrame += 1;
		pa -= RAD_PER_FRAME;
		if (pa < 0) {
			pa += 2 * PI;
		}
		pdx = cos(pa) * 3;
		pdy = sin(pa) * 3;
	}

	if (state[SDL_SCANCODE_D]) {
		currentFrame -= 1;
		pa += RAD_PER_FRAME;
		if (pa > 2 * PI) {
			pa -= 2 * PI;
		}
		pdx = cos(pa) * 3;
		pdy = sin(pa) * 3;
	}

	if (state[SDL_SCANCODE_W]) {
		if (map[int((floor(yPos + pdy) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0 && map[int((floor(yPos + pdy) / w_tille)) * mapX + int(floor((xPos + 32) / w_tille))] == 0 && map[int((floor(yPos + pdy+32) / w_tille)) * mapX + int(floor((xPos+32) / w_tille))] == 0 && map[int((floor(yPos + pdy+32) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0) {
			yPos += pdy;
		}
		if (map[int((floor(yPos) / w_tille)) * mapX + int(floor((xPos + pdx) / w_tille))] == 0 && map[int((floor(yPos) / w_tille)) * mapX + int(floor((xPos + pdx + 32) / w_tille))] == 0 && map[int((floor(yPos + 32) / w_tille)) * mapX + int(floor((xPos + pdx + 32) / w_tille))] == 0 && map[int((floor(yPos + 32) / w_tille)) * mapX + int(floor((xPos + pdx) / w_tille))] == 0) {
			xPos += pdx;
		}
	}

	if (state[SDL_SCANCODE_S]) {
		if (map[int((floor(yPos - pdy) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0 && map[int((floor(yPos - pdy) / w_tille)) * mapX + int(floor((xPos + 32) / w_tille))] == 0 && map[int((floor(yPos - pdy + 32) / w_tille)) * mapX + int(floor((xPos + 32) / w_tille))] == 0 && map[int((floor(yPos - pdy + 32) / w_tille)) * mapX + int(floor((xPos) / w_tille))] == 0) {
			yPos -= pdy;
		}
		if (map[int((floor(yPos) / w_tille)) * mapX + int(floor((xPos - pdx) / w_tille))] == 0 && map[int((floor(yPos) / w_tille)) * mapX + int(floor((xPos - pdx + 32) / w_tille))] == 0 && map[int((floor(yPos + 32) / w_tille)) * mapX + int(floor((xPos - pdx + 32) / w_tille))] == 0 && map[int((floor(yPos + 32) / w_tille)) * mapX + int(floor((xPos - pdx) / w_tille))] == 0) {
			xPos -= pdx;
		}
	}
}

void raycasting(int mapX, int mapY, int mapS, int* map, double xPos, double yPos, double pa, SDL_Renderer* renderer) {
	


	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderDrawLine(renderer, xPos + 16, yPos + 16, 0, 0);
}

int main(int argc, char** argv){
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	bool isRunning = true;

	SDL_GetRendererOutputSize(renderer, &mapS, &mapS); // Gets the width of the screen

	int w_tille = mapS / mapX; // Gets the width of a tille


	SDL_Rect player_rect = { 100,100,25,25};
	int player_speed = 6;
 
	const int PLAYER_WIDTH = 8;   // Width of each frame in the texture
	const int PLAYER_HEIGHT = 8;  // Height of each frame in the texture
	const int PLAYER_FRAME_COUNT = 32; // Total number of frames in the texture
	const int FRAMES_PER_ROW = 32; // Number of frames per row in the texture

	const double ANGLE_PER_FRAME = static_cast<double>(360) / FRAMES_PER_ROW;
	const double RAD_PER_FRAME = PI * ANGLE_PER_FRAME / 180;

	int currentFrame = 0;


	static int lastTime;

	// Variables to store the player's position
	pdx = cos(pa) * 5;
	pdy = sin(pa) * 5;

	SDL_Surface* playerSurface = IMG_Load("C:\\Users\\Petr\\Desktop\\player.png");
	SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_FreeSurface(playerSurface);


	while (isRunning) {
		// Limiting the FPS
		lastFrame = SDL_GetTicks();
		if (lastFrame >= (lastFrame + 1000)) {
			lastTime = lastFrame;
			fps = frameCount;
			frameCount = 0;

		}
		frameCount++;
		int timerFPS = SDL_GetTicks() - lastFrame;
		if (timerFPS < (1000 / 60)) {
			SDL_Delay((1000 / 60)-timerFPS);
		}


		Input(isRunning, player_speed, w_tille, mapX, mapY, mapS, map, xPos, yPos, pdx, pdy, pa, PLAYER_WIDTH, PLAYER_HEIGHT, currentFrame, RAD_PER_FRAME); // Movement, QUIT

		SDL_RenderClear(renderer);

		DrawMap(renderer, mapX, mapY, mapS, map, player_rect); // Draws the map

		//raycasting(mapX, mapY, mapS, map, xPos, yPos, pa, renderer);
 

		 // Calculate the source rectangle based on the current frame
		int srcX = round(currentFrame * PLAYER_WIDTH);
		int srcY = 0; // Since all frames are on a single row, y is always 0

		// Set up the source and destination rectangles
		SDL_Rect srcRect = { srcX, srcY, PLAYER_WIDTH, PLAYER_HEIGHT };
		SDL_Rect destRect = { xPos, yPos, PLAYER_WIDTH * 4, PLAYER_HEIGHT * 4};

		// Render the player's current frame
		SDL_RenderCopy(renderer, playerTexture, &srcRect, &destRect);

		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderDrawLine(renderer, xPos + 16, yPos + 16, xPos + 16 + pdx*50, yPos + 16 + pdy*50);

		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
