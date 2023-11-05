#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	// INIT
	int window_w = 1460;
	int window_h = 960;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Defense Kingdom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, false);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}

	// FPS SETTING
	const int FPS = 100; // !!!!!FPS_MAX = 100!!!!! //
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	Uint32 frameTime;

	// LOAD IMAGES AND DEFINE IMAGE RECT
	SDL_Surface* tmpSurface;

	tmpSurface = IMG_Load("assets/Blue.png");
	SDL_Texture* TmpTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	SDL_Rect TmpRect{};
	TmpRect.w = 64;
	TmpRect.h = 64;
	TmpRect.x = 0;
	TmpRect.y = 0;
	double TmpRect_Speed = 0.1; // !!!!! SPEED_MIN = 1/frameDelay !!!!! //

	short int TmpRect_XDir = 1;
	short int TmpRect_YDir = 1;

	// MAIN LOOP
	bool isRunning = true;
	SDL_Event event;

	while (isRunning)
	{
		// fps 1
		frameStart = (Uint32)SDL_GetTicks64();

		// event
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
		}

		// update
		TmpRect.x += (int)(TmpRect_Speed * frameDelay);
		TmpRect.y += (int)(TmpRect_Speed * frameDelay);

		// render
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, TmpTex, NULL, &TmpRect);
		SDL_RenderPresent(renderer);

		// fps 2
		frameTime = (Uint32)SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	
	return 0;
}