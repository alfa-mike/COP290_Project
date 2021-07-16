
#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"

GameObject* player;
GameObject* monster;

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

	player = new GameObject("src/player1.png",renderer,0,0);
	monster = new GameObject("src/monster2.png",renderer,50,50);
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
	player->gUpdate();
	monster->gUpdate();
}

void Game::render()
{
	SDL_RenderClear(renderer);
	player->gRender();
	monster->gRender();
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}