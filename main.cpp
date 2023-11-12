#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#define WINDOW_W 1460
#define WINDOW_H 960

#define MONSTERLIMIT 50

bool collision(SDL_Rect A, SDL_Rect B);
typedef struct _Vector
{
	double x = 0;
	double y = 0;
} Vector;

typedef struct _Monster
{
	SDL_Rect rect{};
	Vector speed{};
	bool OnScreen = false;
} MONSTER;

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
	// !!!!!FPS_MAX = 100!!!!! //
	const int FPS = 100;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	Uint32 frameTime;

	// LOAD IMAGES AND DEFINE IMAGE RECT
	// !!!!! SPEED_MIN = 1/frameDelay !!!!! //
	SDL_Surface* tmpSurface;

	// Monster
	MONSTER* monster = (MONSTER*)malloc(sizeof(MONSTER) * MONSTERLIMIT);
	tmpSurface = IMG_Load("assets/Blue.png");
	SDL_Texture* MonsterTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	
	if (monster != NULL)
	{
		for (int i = 0; i < MONSTERLIMIT; i++)
		{
			monster[i].rect.w = 64;
			monster[i].rect.h = 64;
			monster[i].rect.x = -i * 128;
			monster[i].rect.y = WINDOW_H / 2 - monster[i].rect.h / 2;
			monster[i].speed.x = 2;
			monster[i].speed.y = 0;
		}
	}	
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
		
		if (monster != NULL)
		{
			for (int i = 0; i < MONSTERLIMIT; i++)
			{
				monster[i].rect.x += (int)monster[i].speed.x;
				monster[i].rect.y += (int)monster[i].speed.y;
			}
		}


		// render
		SDL_RenderClear(renderer);
		if (monster != NULL)
		{
			for (int i = 0; i < MONSTERLIMIT; i++)
			{
				SDL_RenderCopy(renderer, MonsterTex, NULL, &monster[i].rect);
			}
		}
		//SDL_RenderCopy(renderer, CTmpTex, NULL, &CTmpRect);
		SDL_RenderPresent(renderer);

		// fps 2
		frameTime = (Uint32)SDL_GetTicks64() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	
	// EXIT

	// memory
	if (monster != NULL)
	{
		free(monster);
		monster = NULL;
	}

	// sdl
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
