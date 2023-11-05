#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <random>

#define MAX_TOWERS 2
#define MAX_ENEMIES 5
#define MAX_BULLETS 100

bool CheckCollision(SDL_Rect* p1, SDL_Rect* p2);
float GetDistance(SDL_Rect* p1, SDL_Rect* p2);
float GetDistance(int x, int y);

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
	SDL_Texture* TowerTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	tmpSurface = IMG_Load("assets/orenge.png");
	SDL_Texture* EnemyTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	tmpSurface = IMG_Load("assets/purple.png");
	SDL_Texture* BulletTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	SDL_FreeSurface(tmpSurface);
	// End of Texture

	// Rectangle
	SDL_Rect backgroundRect{0,0,window_w,window_h};
	SDL_Rect TowerRect[MAX_TOWERS]{};
	SDL_Rect EnemyRect[MAX_ENEMIES]{};
	SDL_Rect BulletRect[MAX_BULLETS]{};
	SDL_Point BulletDir[MAX_BULLETS]{};

	bool bTowerOnScreen[MAX_TOWERS]{};
	bool bEnemyOnScreen[MAX_ENEMIES]{};
	bool bBulletOnScreen[MAX_BULLETS]{};
	bool bTowerBusy[MAX_TOWERS]{};
	


	for (int i = 0; i < MAX_TOWERS; i++)
	{
		bTowerOnScreen[i] = true;
	}

	int nPad = 300;
	Uint32 format;
	int access;
	int w;
	int h;

	SDL_QueryTexture(TowerTexture, &format, &access, &w, &h);
	for (int i = 0; i < MAX_TOWERS; i++)
	{
		TowerRect[i].x = (i+1)*400;
		TowerRect[i].y = 200;
		TowerRect[i].w = w;
		TowerRect[i].h = h;
	}

	SDL_QueryTexture(EnemyTexture, &format, &access, &w, &h);
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		EnemyRect[i].w = w;
		EnemyRect[i].h = h;
	}

	SDL_QueryTexture(BulletTexture, &format, &access, &w, &h);
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		BulletRect[i].w = w;
		BulletRect[i].h = h;
	}

	// End of Rectangle

	// dummy variables
	int nCount = 0;
	int nSpeed = 20;
	Uint32 nCountFrame = 0;
	int nEnemyRate = 100;
	int nEnemyIdx = 0;
	int nFireRate = 50;
	int nBulletIdx = 0;
	int nMaxRange = 400;

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
		// appear an Enemy
		if (nCountFrame % nEnemyRate == 0)
		{
			if (nEnemyIdx < MAX_ENEMIES)
			{
				bEnemyOnScreen[nEnemyIdx] = true;
				nEnemyIdx++;
			}
		}
		// Move Enemy
		for (int i = 0; i < MAX_ENEMIES; i++)
		{
			if (bEnemyOnScreen[i])
			{
				EnemyRect[i].x += 5;
				EnemyRect[i].y = window_h / 2;
			}
		}

		// Check Enemy condition
		for (int i = 0; i < MAX_ENEMIES; i++)
		{
			if (bEnemyOnScreen[i])
			{
				for (int j = 0; j < MAX_BULLETS; j++)
				{
					if (bBulletOnScreen[j])
					{
						if (CheckCollision(&EnemyRect[i], &BulletRect[j]))
						{
							bEnemyOnScreen[i] = false;
							bBulletOnScreen[j] = false;
							break;
						}
					}
				}
			}
		}

		// fire Bullet on Tower
		for (int i = 0; i < MAX_TOWERS; i++)
		{
			if (nCountFrame % nFireRate == 0)
			{
				bTowerBusy[i] = false;
			}

			if (bTowerOnScreen[i] && !bTowerBusy[i])
			{
				// find empty magazine and fire bullet
				for (int k = 0; k < MAX_BULLETS; k++)
				{
					if (bBulletOnScreen[k] == false)
					{
						nBulletIdx = k;
						bBulletOnScreen[nBulletIdx] = true;
						break;
					}
				}
				// find a Nearest Enemy
				float fDistance1 = 2000.;
				int nNearestEnemyIdx = 0;
				for (int j = 0; j < MAX_ENEMIES; j++)
				{
					if (bEnemyOnScreen[j])
					{
						float fDistance2 = GetDistance(&TowerRect[i], &EnemyRect[j]);
						if (fDistance2 > nMaxRange)
						{
							break;
						}
						if (fDistance1 > fDistance2)
						{
							fDistance1 = fDistance2;
							nNearestEnemyIdx = j;
							BulletDir[nBulletIdx].x = EnemyRect[j].x - TowerRect[i].x;
							BulletDir[nBulletIdx].y = EnemyRect[j].y - TowerRect[i].y;
							BulletRect[nBulletIdx].x = TowerRect[i].x + TowerRect[i].w / 2 - BulletRect[nBulletIdx].w / 2;
							BulletRect[nBulletIdx].y = TowerRect[i].y + TowerRect[i].h / 2 - BulletRect[nBulletIdx].h / 2;
							bTowerBusy[i] = true;
						}
					}
				}
			}
		}
		
		// Move bullet
		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (bBulletOnScreen[i])
			{
				float d = GetDistance(BulletDir[i].x, BulletDir[i].y);
				BulletRect[i].x += (int)(nSpeed * BulletDir[i].x / d);
				BulletRect[i].y += (int)(nSpeed * BulletDir[i].y / d);
			}
		}


		// Render
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
		for (int i = 0; i < MAX_TOWERS; i++)
		{
			if (bTowerOnScreen[i])
			{
				SDL_RenderCopy(renderer, TowerTexture, NULL, &TowerRect[i]);
			}
		}
		for (int i = 0; i < MAX_ENEMIES; i++)
		{
			if (bEnemyOnScreen[i])
			{
				SDL_RenderCopy(renderer, EnemyTexture, NULL, &EnemyRect[i]);
			}
		}
		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (bBulletOnScreen[i])
			{
				SDL_RenderCopy(renderer, BulletTexture, NULL, &BulletRect[i]);
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

		nCountFrame++;
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
		printf("collision\n");
		return true;
	}
	else
	{
		return false;
	}
}

float GetDistance(SDL_Rect* p1, SDL_Rect* p2)
{
	float p1x = p1->x + p1->w / 2.f;
	float p1y = p1->y + p1->h / 2.f;
	float p2x = p2->x + p2->w / 2.f;
	float p2y = p2->y + p2->h / 2.f;
	return sqrt((p2x - p1x) * (p2x - p1x) - (p2y - p1y) * (p2y - p1y));
}

float GetDistance(int x, int y)
{
	return (float)sqrt(x * x + y * y);
}