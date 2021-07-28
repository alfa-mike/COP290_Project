#pragma once
#include "GameObject.h"
#define IMG_MAZEDOOR "res/Images/Lock.png"
class MazeDoor :
	public GameObject
{
private:
	static SDL_Texture * doorTexture;
public:
	MazeDoor(std::shared_ptr<Room> setRoom);
	~MazeDoor();
};
