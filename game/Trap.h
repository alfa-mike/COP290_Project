#pragma once
#include "GameObject.h"

#define IMG_TRAP_INACTIVE "res/TrapInactive.png"
#define IMG_TRAP_ACTIVE "res/TrapActive.png"

class MazeTrap :
	public GameObject
{
private:
	int maxTrapTime;
	int curTrapTime;
	static SDL_Texture * trapActiveTexture;
	static SDL_Texture * trapInactiveTexture;
public:
	
	MazeTrap(std::shared_ptr<Room> room);
	~MazeTrap();

	//Increments trap time and checks if trap state must change each cycle
	void NextCycle()
	{
		curTrapTime++;
		if (curTrapTime > maxTrapTime)
		{ 
			curTrapTime = 0;
		}
		SetTrapTexture();
	}
	
	//Sets trap texture based off of current trap state
	void SetTrapTexture()
	{
		if(curTrapTime == 0)
		{
			curObjRoom->roomTypes.push_back(Trap);
			curObjTexture = trapActiveTexture;
		}
		else 
		{
			auto roomTypeIter = std::find(begin((curObjRoom)->roomTypes), end((curObjRoom)->roomTypes), Trap);
			if (roomTypeIter != end(curObjRoom->roomTypes))
			{
				curObjRoom->roomTypes.erase(roomTypeIter);
				curObjTexture = trapInactiveTexture;
			}
		}
	}
};
