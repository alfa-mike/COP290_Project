#pragma once
#include "GameObject.h"

#define IMG_MAZEKEY "res/Key.png"
class MazeKey :
	public GameObject
{
private:
	static SDL_Texture * keyTexture;
public:
	MazeKey(std::shared_ptr<Room> room);
	~MazeKey();
};
