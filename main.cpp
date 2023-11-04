#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <random>

#define MAX_ENTITIES 100

bool CheckCollision(SDL_Rect* p1, SDL_Rect* p2);

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
	tmpSurface = IMG_Load("assets/orenge.png");
	SDL_Texture* sample2Texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	tmpSurface = IMG_Load("assets/purple.png");
	SDL_Texture* sample3Texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	SDL_FreeSurface(tmpSurface);
	// End of Texture

	// Rectangle
	SDL_Rect backgroundRect{0,0,window_w,window_h};
	SDL_Rect arrayRect[MAX_ENTITIES]{};

	bool bOnScreen[MAX_ENTITIES]{};
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		bOnScreen[i] = true;
	}

	int nPad = 300;
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		arrayRect[i].x = rand() % (window_w - 2*nPad) + nPad;
		arrayRect[i].y = rand() % (window_h - 2*nPad) + nPad;
		arrayRect[i].w = 10;
		arrayRect[i].h = 10;
	}
	// End of Rectangle

	// dummy variables
	int nCount = 0;
	int nSpeed = 21;

	// Main Loop
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

		/* Keyboard event */
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				isRunning = false;
				break;
			}
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				break;
			}
		}

		// Update
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (bOnScreen[i])
			{
				arrayRect[i].x += rand() % nSpeed - nSpeed/2;
				arrayRect[i].y += rand() % nSpeed - nSpeed / 2;
			}
		}
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (bOnScreen[i])
			{
				for (int j = 0; j < i; j++)
				{
					if (bOnScreen[j])
					{
						if (CheckCollision(&arrayRect[i], &arrayRect[j]))
						{
							bOnScreen[i] = false;
							bOnScreen[j] = false;
							nCount++;
							printf("%d Collision %d and %d\n", nCount, i, j);
							break;
						}
					}
				}
			}
		}

		// Render
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			if (bOnScreen[i])
			{
				SDL_RenderCopy(renderer, sample3Texture, NULL, &arrayRect[i]);
			}
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
			//printf("FPS : %d\n", (int)(1/(((Uint32)SDL_GetTicks64() - frameStart) / 1000.f)));
		}
	}

	// Close SDL
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	
	return 0;
}

bool CheckCollision(SDL_Rect* p1, SDL_Rect* p2)
{
	if (p1->x < p2->x && p1->x + p1->w > p2->x && p1->y < p2->y && p1->y + p1->h > p2->y)
	{
		return true;
	}
	else
	{
		return false;
	}
}