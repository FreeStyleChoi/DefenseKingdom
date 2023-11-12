#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#define WINDOW_W 1460
#define WINDOW_H 960

bool collision(SDL_Rect A, SDL_Rect B);

int main(int argc, char** argv)
{
	// INIT
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("Defense Kingdom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, false);
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

	// Tmp
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

	// CTmp
	tmpSurface = IMG_Load("assets/Red.png");
	SDL_Texture* CTmpTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	SDL_Rect CTmpRect{};
	CTmpRect.w = 64;
	CTmpRect.h = 64;
	CTmpRect.x = WINDOW_W - CTmpRect.w;
	CTmpRect.y = 0;
	double CTmpRect_Speed = 0.1; // !!!!! SPEED_MIN = 1/frameDelay !!!!! //

	short int CTmpRect_XDir = -1;
	short int CTmpRect_YDir = 1;

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
		TmpRect.x += TmpRect_XDir * (int)(TmpRect_Speed * frameDelay);
		TmpRect.y += TmpRect_YDir * (int)(TmpRect_Speed * frameDelay);

		CTmpRect.x += CTmpRect_XDir * (int)(CTmpRect_Speed * frameDelay);
		CTmpRect.y += CTmpRect_YDir * (int)(CTmpRect_Speed * frameDelay);
		
		/*collision*/

		if (collision(TmpRect, CTmpRect))
		{
			TmpRect_XDir *= -1;
			CTmpRect_XDir *= -1;
		}


		// render
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, TmpTex, NULL, &TmpRect);
		SDL_RenderCopy(renderer, CTmpTex, NULL, &CTmpRect);
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

bool collision(SDL_Rect A, SDL_Rect B)
{
	bool Cx = A.x + A.w >= B.x && A.x <= B.x + B.w;
	bool Cy = A.y + A.h >= B.y && A.y <= B.y + B.h;
	bool resault = false;

	if (Cx && Cy)
	{
		resault = true;
	}

	return resault;
}
