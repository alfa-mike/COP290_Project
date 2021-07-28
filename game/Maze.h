 
#pragma once

#include <random>

#include "Room.h"	
#include "Trap.h"
#include "Key.h"
#include "MazeDoor.h"
#include "Guard.h"

//When To Start Obstacles
#define TRAPS_START 2
#define GUARDS_START 4

//Random Element Function
template <typename T, typename A>
T randomElement(std::vector<T, A> &vec)
{
	std::random_device random_device;
	std::mt19937 engine{ random_device() };
	std::uniform_int_distribution<int> dist(0, vec.size() - 1);
	T element = vec[dist(engine)];
	return element;
}

class Maze
{

private:
	
	int mazeX_RoomCount;
	int mazeY_RoomCount;
	
	
	int mazeX_Offset;
	int mazeY_Offset;
	int mazeX_Size;
	int mazeY_Size;

	
	int obstacleSpacing = 3;	

	
	std::vector<std::shared_ptr<Room>> allRooms;	
	
	
	std::vector <std::shared_ptr<Room>> obstacleRooms;
	
	
	std::vector<std::shared_ptr<GameObject>> objectsInMaze;

	
	SDL_Rect mazeRect;	

	
	SDL_Renderer * mazeRenderer = NULL;

public:	
	int mazeLevel;	//Current Game Level

	
	std::shared_ptr<MazeKey> mazeKeyPtr;
	
	std::shared_ptr<MazeDoor> mazeDoorPtr;	
	
	Position startPos;
	Position finalPos;

	//Constructors and Destructors
	Maze(int xCount, int yCount, int xOffset, int yOffset, int xSize, int ySize, SDL_Renderer * renderer, int level, bool showCreation);
	Maze(const Maze &other);
	Maze();
	~Maze();
	
	
	void ResetMaze()
	{
		//Clear all previous vectors
		allRooms.clear();
		obstacleRooms.clear();
		objectsInMaze.clear();
		
		//Create Maze and Items in Maze
		CreateRooms();
		CarveMaze();
		if (mazeLevel > TRAPS_START)
		{
			CreateObjects();
		}
		CreateKey();
	}

	
	void SetMaze(int x, int y, int level)
	{
		mazeX_RoomCount = x;
		mazeY_RoomCount = y;
		mazeLevel = level;
		ResetMaze();
	}

	//Sets the maze up for the next level
	void NextLevelMaze()
	{
		mazeLevel++;
		mazeX_RoomCount++;
		mazeY_RoomCount++;
		ResetMaze();
	}

	
	void ChangePosition(Position &pos, directions dir)
	{
		switch (dir)
		{
		case up:
			pos.yPos--;
			break;
		case left:
			pos.xPos--;
			break;
		case down:
			pos.yPos++;
			break;
		case right:
			pos.xPos++;
			break;
		}
	}

	
	void CreateRooms()
	{
		directions allDir[] = { up, left, down, right };

		//Creates all the rooms
		for (int x = 0; x < mazeX_RoomCount; x++)
		{
			for (int y = 0; y < mazeY_RoomCount; y++)
			{
				std::shared_ptr<Room> newPtr(new Room(Position(x, y), mazeRenderer));
				newPtr->MakeRoomRect(mazeX_Offset, mazeY_Offset, mazeX_RoomCount, mazeY_RoomCount, mazeX_Size, mazeY_Size);
				newPtr->AddRoomToRenderer(10);
				allRooms.push_back(newPtr);
			}
		}

		
		for_each(begin(allRooms), end(allRooms), [&](std::shared_ptr<Room> &curRoomPtr)
		{
			std::for_each(std::begin(allDir), std::end(allDir), [&](directions dir)
			{
				Position testPos = curRoomPtr->roomPos;
				ChangePosition(testPos, dir);
				auto adjRoom = find_if(begin(allRooms), end(allRooms), [&](std::shared_ptr<Room> &checkRoomPtr)
				{
					return checkRoomPtr->roomPos == testPos;
				});
				if (adjRoom != end(allRooms))
				{
					curRoomPtr->adjRooms.push_back((*adjRoom));
					curRoomPtr->availRooms.push_back((*adjRoom));
				}
			});
		});
	}

	
	void CarveMaze()
	{
		
		std::vector<std::shared_ptr<Room>> currentPath;
		
		
		size_t longestPath = 0;

		
		int inMazeCounter = 0;

		
		std::shared_ptr<Room> curRoomPtr = randomElement(allRooms);
		startPos = curRoomPtr->roomPos;
		curRoomPtr->roomTypes.push_back(Start);

		while (inMazeCounter < mazeX_RoomCount * mazeY_RoomCount)
		{
			
			if (!curRoomPtr->inMaze)
			{
				
				currentPath.push_back(curRoomPtr);
				//Set it as in the maze
				curRoomPtr->inMaze = true;
				
				inMazeCounter++;
				
				for_each(begin(curRoomPtr->adjRooms), end(curRoomPtr->adjRooms), [&](std::shared_ptr<Room>  adjRoomPtr)
				{
					//Find if the current room is in the available rooms of the current adjacent room
					auto roomToRemoveIter = std::find(begin(adjRoomPtr->availRooms), end(adjRoomPtr->availRooms), curRoomPtr);
					//If the room is found, remove it from the available rooms
					if (roomToRemoveIter != end(adjRoomPtr->availRooms))
					{
						adjRoomPtr->availRooms.erase(roomToRemoveIter);
					}
				});

				
				if (longestPath < currentPath.size())
				{
					longestPath = currentPath.size();
					finalPos = curRoomPtr->roomPos;
				}

				
				if(mazeLevel > TRAPS_START)
				{
					
					if (currentPath.size() % obstacleSpacing == 0)
					{
						obstacleRooms.push_back(curRoomPtr);
					}
				}
			}

			
			if (!curRoomPtr->availRooms.empty())
			{
				
				std::shared_ptr <Room> nextRoomPtr = randomElement(curRoomPtr->availRooms);	
				
				//Connects the next room to the current one and vice versa
				curRoomPtr->ConnectRoom(nextRoomPtr);
				nextRoomPtr->ConnectRoom(curRoomPtr);

				
				curRoomPtr->AssignRoomTextures();
				nextRoomPtr->AssignRoomTextures();

				curRoomPtr->AddRoomToRenderer(10);
				nextRoomPtr->AddRoomToRenderer(10);

				//Removes the next room from the available rooms for the current room 
				curRoomPtr->availRooms.erase(std::find(begin(curRoomPtr->availRooms), end(curRoomPtr->availRooms), nextRoomPtr));	
				
																																	
				curRoomPtr = nextRoomPtr;	
			}
			
			else
			{
				currentPath.pop_back();
				curRoomPtr = currentPath.back();
			}
		}

		//Places the exit to the maze
		std::shared_ptr<Room> finalRoom = FindRoomByPos(finalPos);
		mazeDoorPtr = std::shared_ptr<MazeDoor>(new MazeDoor(finalRoom));
		finalRoom->roomTypes.push_back(Final);

	}

	
	void CreateObjects()
	{
		for_each(begin(obstacleRooms), end(obstacleRooms), [&](std::shared_ptr<Room> curRoomPtr) {
			if(curRoomPtr->roomTypes.size() == 0)
			{
				//If difficulty is 2 or higher and the room has 3 connected rooms, create a guard
				if (curRoomPtr->connectRooms.size() == 3 && mazeLevel > GUARDS_START)
				{
					std::shared_ptr<MazeGuard> mazeGuardPtr = std::shared_ptr<MazeGuard>(new MazeGuard(curRoomPtr));
					objectsInMaze.push_back(std::static_pointer_cast<GameObject>(mazeGuardPtr));
				}
				//Otherwise, if the difficulty is 1 or higher, create a Trap
				else
				{
					std::shared_ptr<MazeTrap> mazeTrapPtr = std::shared_ptr<MazeTrap>(new MazeTrap(curRoomPtr));
					objectsInMaze.push_back(std::static_pointer_cast<GameObject>(mazeTrapPtr));
				}
			}
		});
	}

	void CreateKey()
	{
		std::shared_ptr<Room> curRoomPtr;
		do
		{
			curRoomPtr = randomElement(allRooms);
		} while (curRoomPtr->roomTypes.size() > 0);
		
		mazeKeyPtr = std::shared_ptr<MazeKey>(new MazeKey(curRoomPtr));
		curRoomPtr->roomTypes.push_back(Key);
	}

	//Find a room by the position
	std::shared_ptr<Room> FindRoomByPos(Position pos)
	{
		auto iter = (std::find_if(begin(allRooms), end(allRooms), [&](std::shared_ptr<Room> checkRoom) {
			return checkRoom->roomPos == pos;
		}));
		if (iter != end(allRooms))
		{
			return *iter;
		}
		else
		{
			return nullptr;
		}
	}

	
	void MazeOutline()
	{
		SDL_Color color = { 255, 255, 255, 255 };
		SDL_RenderDrawRect(mazeRenderer, &mazeRect);
	}

	//Adds each room and an outline to the renderer
	void AddMazeRoomsToRenderer(int delay)
	{
		for_each(begin(allRooms), end(allRooms), [&](std::shared_ptr<Room> curRoomPtr)
		{
			curRoomPtr->AddRoomToRenderer(delay);
		});
		MazeOutline();
	}
	
	
	void AddMazeObstaclesToRenderer()
	{
		if (mazeLevel > TRAPS_START)
		{
			for_each(begin(objectsInMaze), end(objectsInMaze), [&](std::shared_ptr<GameObject> curObjPtr)
			{
				curObjPtr->AddObjToRenderer();
			});
		}
	}

	
	void NextMazeCycle()
	{
		for_each(begin(objectsInMaze), end(objectsInMaze), [&](std::shared_ptr<GameObject> curObjPtr)
		{
			curObjPtr->NextCycle();
		});
	}
};
