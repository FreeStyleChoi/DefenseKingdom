#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <random>

#define MAX_TOWERS 100
#define MAX_ENEMIES 100
#define MAX_BULLETS 100

bool CheckCollision(SDL_Rect* p1, SDL_Rect* p2);
float GetDistance(SDL_Rect* p1, SDL_Rect* p2);
float GetDistance(int x, int y);
int GenTower(SDL_Rect (&Tower)[MAX_TOWERS], bool (&bTowerOnScreen)[MAX_TOWERS], int nTowerIdx, int nPosX, int nPosY);

int main(int argc, char** argv)
{
	int window_w = 800;
	int window_h = 600;

	const int FPS = 50;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart, frameTime;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(
		"Defense Kingdom", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_w, window_h, false);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	bool isRunning = true;
	SDL_Event event;

	// Texture
	SDL_Surface* tmpSurface;

	tmpSurface = IMG_Load("assets/background.png");
	SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	tmpSurface = IMG_Load("assets/green.png");
	SDL_Texture* TowerTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	tmpSurface = IMG_Load("assets/gray.png");
	SDL_Texture* TowerTextureBusy = SDL_CreateTextureFromSurface(renderer, tmpSurface);
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
	SDL_Point EnemyDir[MAX_ENEMIES]{};
	SDL_Rect BulletRect[MAX_BULLETS]{};
	SDL_Point BulletDir[MAX_BULLETS]{};

	bool bTowerOnScreen[MAX_TOWERS]{};
	bool bEnemyOnScreen[MAX_ENEMIES]{};
	bool bBulletOnScreen[MAX_BULLETS]{};
	int nTowerBusy[MAX_TOWERS]{};

	int nTowerIdx = 0;

	Uint32 format;
	int access;
	int w;
	int h;

	SDL_QueryTexture(TowerTexture, &format, &access, &w, &h);
	for (int i = 0; i < MAX_TOWERS; i++)
	{
		TowerRect[i].x = (i+1)*350;
		TowerRect[i].y = 150;
		TowerRect[i].w = w;
		TowerRect[i].h = h;
	}

	SDL_QueryTexture(EnemyTexture, &format, &access, &w, &h);
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		EnemyRect[i].w = w;
		EnemyRect[i].h = h;
		EnemyRect[i].y = rand() % (window_h-h);
	}

	SDL_QueryTexture(BulletTexture, &format, &access, &w, &h);
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		BulletRect[i].w = w;
		BulletRect[i].h = h;
	}

	// End of Rectangle

	// dummy variables
	int nBulletSpeed = 10;
	int nEnemySpeed = 10;
	Uint32 nCountFrame = 0;
	int nEnemyRate = 10;
	int nEnemyIdx = 0;
	//int nFireRate = 3;
	int nTowerReload = 20;
	int nBulletIdx = 0;
	int nMaxRange = 400;
	int numBullets = 0;

	int key_dir_left = 0;
	int key_dir_right = 0;
	int key_dir_up = 0;
	int key_dir_down = 0;


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
			case SDLK_LEFT:
				key_dir_left = -1;
				break;
			case SDLK_RIGHT:
				key_dir_right = 1;
				break;
			case SDLK_UP:
				key_dir_up = -1;
				break;
			case SDLK_DOWN:
				key_dir_down = 1;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				break;
			case SDLK_LEFT:
				key_dir_left = 0;
				break;
			case SDLK_RIGHT:
				key_dir_right = 0;
				break;
			case SDLK_UP:
				key_dir_up = 0;
				break;
			case SDLK_DOWN:
				key_dir_down = 0;
				break;
			}
			break;

		/* Mouse event */
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				//printf("click mouse button : %d, %d\n", event.button.x, event.button.y);
				nTowerIdx = GenTower(TowerRect, bTowerOnScreen, nTowerIdx, event.button.x, event.button.y);
				break;
			}
			break;
		}

		//printf("l, r, u, d : %d, %d, %d, %d\n", key_dir_left, key_dir_right, key_dir_up, key_dir_down);

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
			if (bEnemyOnScreen[i] && MAX_TOWERS == 1 && bTowerOnScreen[0])
			{
				// get direction between Tower and Enemy
				// get distance 
				// update Enemy position

			}
			else if (bEnemyOnScreen[i])
			{
				EnemyRect[i].x += nEnemySpeed;
				//EnemyRect[i].y = window_h / 2;

				// Remove an Enemy out of Screen
				if (EnemyRect[i].x + EnemyRect[i].w < 0 || EnemyRect[i].x > window_w ||
					EnemyRect[i].y + EnemyRect[i].h < 0 || EnemyRect[i].y > window_h)
				{
					bEnemyOnScreen[i] = false;
				}
			}
		}

		// A Tower control by keyboard
		if (MAX_TOWERS == 1 && bTowerOnScreen[0])
		{
			TowerRect[0].x += (key_dir_left + key_dir_right) * 3;
			TowerRect[0].y += (key_dir_up + key_dir_down) * 3;
		}

		// Check collision between Enemy and bullet
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
							numBullets--;
							break;
						}
					}
				}
			}
		}

		// fire Bullet on Tower
		for (int i = 0; i < MAX_TOWERS; i++)
		{
			// Reset a rest time of Tower
			if (bTowerOnScreen[i] && nTowerBusy[i] > 0)
			{
				nTowerBusy[i]--;
				continue;
			}

			if (bTowerOnScreen[i] && nTowerBusy[i] == 0)
			{
				// find a Nearest Enemy
				float fDistancePre = (float)(window_w + window_h);
				int nNearestEnemyIdx = -1;
				for (int j = 0; j < MAX_ENEMIES; j++)
				{
					if (bEnemyOnScreen[j])
					{
						float fDistanceCurrent = GetDistance(&TowerRect[i], &EnemyRect[j]);
						if (fDistanceCurrent < nMaxRange && fDistanceCurrent < fDistancePre)
						{
							fDistancePre = fDistanceCurrent;
							nNearestEnemyIdx = j;
							//printf("Min Distance : %f\n", fDistancePre);
						}
					}
				}

				if (nNearestEnemyIdx != -1)
				{
					// find empty magazine and fire bullet
					for (int k = 0; k < MAX_BULLETS; k++)
					{
						if (bBulletOnScreen[k] == false)
						{
							bBulletOnScreen[k] = true;
							nBulletIdx = k;
							numBullets++;
							break;
						}
					}
					// Set Bullet Direction and Bullet Starting Position
					BulletDir[nBulletIdx].x = EnemyRect[nNearestEnemyIdx].x + EnemyRect[nNearestEnemyIdx].w / 2 - TowerRect[i].x - TowerRect[i].w / 2;
					BulletDir[nBulletIdx].y = EnemyRect[nNearestEnemyIdx].y + EnemyRect[nNearestEnemyIdx].h / 2 - TowerRect[i].y - TowerRect[i].h / 2;
					BulletRect[nBulletIdx].x = TowerRect[i].x + TowerRect[i].w / 2 - BulletRect[nBulletIdx].w / 2;
					BulletRect[nBulletIdx].y = TowerRect[i].y + TowerRect[i].h / 2 - BulletRect[nBulletIdx].h / 2;
				
					nTowerBusy[i] = nTowerReload;
				}
			}
		}
		
		// Move bullet
		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (bBulletOnScreen[i])
			{
				float d = GetDistance(BulletDir[i].x, BulletDir[i].y);
				BulletRect[i].x += (int)(nBulletSpeed * BulletDir[i].x / d);
				BulletRect[i].y += (int)(nBulletSpeed * BulletDir[i].y / d);

				// Remove a Bullet out of Screen
				if (BulletRect[i].x + BulletRect[i].w < 0 || BulletRect[i].x > window_w ||
					BulletRect[i].y + BulletRect[i].h < 0 || BulletRect[i].y > window_h)
				{
					bBulletOnScreen[i] = false;
					numBullets--;
				}
			}
		}
		//printf("numBullet : %d\n", numBullets);
		

		// Render
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
		for (int i = 0; i < MAX_TOWERS; i++)
		{
			if (bTowerOnScreen[i])
			{
				if (nTowerBusy[i] == 0)
				{
					SDL_RenderCopy(renderer, TowerTexture, NULL, &TowerRect[i]);
				}
				else
				{
					SDL_RenderCopy(renderer, TowerTextureBusy, NULL, &TowerRect[i]);
				}
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
	if (p1->x < p2->x + p2->w && 
		p1->x + p1->w > p2->x && 
		p1->y < p2->y + p2->h && 
		p1->y + p1->h > p2->y)
	{
		//printf("collision\n");
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
	return sqrt((p2x - p1x) * (p2x - p1x) + (p2y - p1y) * (p2y - p1y));
}

float GetDistance(int x, int y)
{
	return (float)sqrt(x * x + y * y);
}

int GenTower(SDL_Rect (& Tower)[MAX_TOWERS], bool (&bTowerOnScreen)[MAX_TOWERS], int nCurNumTowers, int nPosX, int nPosY)
{
	int nTowerIdx = nCurNumTowers % MAX_TOWERS;

	bTowerOnScreen[nTowerIdx] = true;
	Tower[nTowerIdx].x = nPosX;
	Tower[nTowerIdx].y = nPosY;
	//printf("Tower number : %d\n", nTowerIdx);
	nCurNumTowers++;

	return nCurNumTowers;
}