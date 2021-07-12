
#include "Game.h"

SDL_Texture* playerTexture ; 

Game::Game()
{};

Game::~Game()
{};

void Game::init(const char* wname, int width, int height, bool fullscreen)
{
	int wmode_flag = 0;
	
	if (fullscreen)
	{
		wmode_flag = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(wname, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, wmode_flag);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}

	SDL_Surface* tempSurface1 = IMG_Load("src/player1.png") ; 
	SDL_Surface* tempSurface2 = IMG_Load("src/player2.png") ; 
	playerTexture = SDL_CreateTextureFromSurface(renderer , tempSurface1) ; 

	SDL_FreeSurface(tempSurface1);
}

void Game::EventsHandler()
{
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update()
{
	cnt++;
	std::cout << cnt << std::endl;
}

void Game::render()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, playerTexture, NULL , NULL);
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}