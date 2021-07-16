#include "Game.h"
#include "GameObject.h"
#include "TextureManager.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{

	
	const int framedelay = 1000/90 ;  //dividing by fps value(90) to get delay

	Uint32 framestart;
	int frametime;  
	game = new Game();
	game->init("GameWindow", 800, 600, false);

	while (game->running())
	{
		framestart = SDL_GetTicks();

		game->EventsHandler();
		game->update();
		game->render();

		frametime = SDL_GetTicks() - framestart;
		//check if frame needs to be delayed
		if (frametime<framedelay){
			SDL_Delay(framedelay-frametime);
		}
	}

	game->clean();
	return 0;
}