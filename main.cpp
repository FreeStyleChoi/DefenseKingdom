#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <random>

int main(int argc, char** argv)
{
	int window_w = 1460;
	int window_h = 960;

	const int FPS = 50;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart, frameTime;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Defense Kingdom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, false);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	bool isRunning = true;
	SDL_Event event;

	// Texture
	SDL_Surface* tmpSurface;

	tmpSurface = IMG_Load("assets/background.png");
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	tmpSurface = IMG_Load("assets/green.png");
	SDL_Texture* sample1Texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_Texture* sample2Texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	tmpSurface = IMG_Load("assets/purple.png");
	SDL_Texture* sample3Texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	SDL_FreeSurface(tmpSurface);
	// End of Texture

	// Rectangle
	SDL_Rect backgroundRect{0,0,window_w,window_h};
	SDL_Rect sample1Rect{0,0,100,100};
	SDL_Rect sample2Rect{1360,0,100,100};
	SDL_Rect arrayRect[100];
	SDL_Rect* arrRect = (SDL_Rect*)malloc(sizeof(SDL_Rect) * 100);
	for (int i = 0; i < 100; i++)
	{
		arrayRect[i].x = 0;
		arrayRect[i].y = 0;
		arrayRect[i].w = 10;
		arrayRect[i].h = 10;
	}
	// End of Rectangle

	while (isRunning)
	{
		// Get Ticks
		frameStart = (Uint32)SDL_GetTicks64();

		// Get Event
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
		}

		// Update
		sample1Rect.x += 5;
		sample1Rect.y += 5;
		sample2Rect.x -= 5;
		sample2Rect.y += 5;
		for (int i = 0; i < 100; i++)
		{
			arrayRect[i].x = rand() % (window_w-100);
			arrayRect[i].y = rand() % (window_h-100);
		}

		// Render
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
		SDL_RenderCopy(renderer, sample1Texture, NULL, &sample1Rect);
		SDL_RenderCopy(renderer, sample2Texture, NULL, &sample2Rect);
		for (int i = 0; i < 100; i++)
		{
			SDL_RenderCopy(renderer, sample3Texture, NULL, &arrayRect[i]);
		}
		SDL_RenderPresent(renderer);

		// Control iteration time for Specific FPS
		frameTime = (Uint32)SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
		if (frameTime > 0)
		{
			printf("FPS : %d\n", (int)(1/(((Uint32)SDL_GetTicks64() - frameStart) / 1000.f)));
		}
	}

	// Close SDL
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	
	return 0;
}