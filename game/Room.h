#pragma once

#include "Position.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define IMG_4BORDER "res/Room_4Border.png"

#define IMG_3BORDER_DLU "res/Room_3Border_D_L_U.png"
#define IMG_3BORDER_LUR "res/Room_3Border_L_U_R.png"
#define IMG_3BORDER_DUR "res/Room_3Border_D_U_R.png"
#define IMG_3BORDER_DLR "res/Room_3Border_D_L_R.png"

#define IMG_2BORDER_LU "res/Room_2Border_L_U.png"
#define IMG_2BORDER_UR "res/Room_2Border_U_R.png"
#define IMG_2BORDER_DL "res/Room_2Border_D_L.png"
#define IMG_2BORDER_DR "res/Room_2Border_D_R.png"
#define IMG_2BORDER_DU "res/Room_2Border_D_U.png"
#define IMG_2BORDER_LR "res/Room_2Border_L_R.png"

#define IMG_1BORDER_D "res/Room_1Border_D.png"
#define IMG_1BORDER_L "res/Room_1Border_L.png"
#define IMG_1BORDER_U "res/Room_1Border_U.png"
#define IMG_1BORDER_R "res/Room_1Border_R.png"

#define IMG_0BORDER "res/Room_0Border.png"

//defines directions and is used to determine which textures to load
//based off of the number of walls
enum directions {down = 0b0001, left = 0b0010, up = 0b0100, right = 0b1000};

//Used to determine what rooms may do in the maze
enum RoomType{None, Start, Final, Key, Trap, Guard };

class Room
{
private:
	static SDL_Texture * roomTextures[];

public:

	std::vector<RoomType> roomTypes;
	
	//Position of Rooms
	Position roomPos;

	//Integer that tells us what direction the walls will have
	int wallDirBit = 0b1111;

	//Tells us if the Room is in the maze
	bool inMaze = false;	
	
	//Rooms adjacent to this room
	std::vector<std::shared_ptr<Room>> adjRooms;

	//Rooms that could be selected to connect to this room when creating the maze
	std::vector<std::shared_ptr<Room>> availRooms;

	//Rooms that are connected to the this room
	std::vector<std::shared_ptr<Room>> connectRooms;	
	
	SDL_Rect roomRect;	
	SDL_Texture * curRoomTexture;
	SDL_Renderer * roomRenderer;

	Room(Position pos, SDL_Renderer * renderer);
	Room(const Room &obj);
	~Room();

	//Create Room Rect
	void MakeRoomRect(int &xOffset, int &yOffset, int & xSplits, int & ySplits, const int &mazeWidth, const int &mazeHeight)
	{
		roomRect = {
			xOffset + (int)((roomPos.xPos)* ceil(mazeWidth / xSplits)),
			yOffset + (int)((roomPos.yPos)* ceil(mazeHeight / ySplits)),
			(int)ceil(mazeWidth / (xSplits)),
			(int)ceil(mazeHeight / (ySplits))
		};
	}

	//Check the direction of an adjacent room
	directions CheckAdjRoomDir(Room &room)
	{
		directions dir;
		if (roomPos.xPos == room.roomPos.xPos)
		{
			if (roomPos.yPos < room.roomPos.yPos)
				dir = down;
			else if (roomPos.yPos > room.roomPos.yPos)
				dir = up;
		}
		else if (roomPos.yPos == room.roomPos.yPos)
		{
			if (roomPos.xPos < room.roomPos.xPos)
				dir = right;

			else if (roomPos.xPos > room.roomPos.xPos)
				dir = left;
		}
		return dir;
	}

	//Assign Texture to this room based off the number of walls (wallDirections)
	void AssignRoomTextures()
	{
		curRoomTexture = roomTextures[wallDirBit];
	}

	//Remove Wall if one is there 
	void RemoveWallDirection(std::shared_ptr<Room> &roomToConnectPtr)
	{
		directions dirToRemove = CheckAdjRoomDir(*roomToConnectPtr);
		wallDirBit -= dirToRemove;
	}

	//Connect Room and Remove Wall Direction
	void ConnectRoom(std::shared_ptr<Room> &roomToConnectPtr)
	{
		connectRooms.push_back(roomToConnectPtr);
		RemoveWallDirection(roomToConnectPtr);
	}

	//Adds room to Renderer. If it has a delay, render after waiting. 
	void AddRoomToRenderer(int delay)
	{
		SDL_RenderCopy(roomRenderer, curRoomTexture, NULL, &roomRect);
		if(delay > 0)
		{
			SDL_Delay(delay);
			SDL_RenderPresent(roomRenderer);
		}
	}

	void AddRoomToRenderer()
	{
		AddRoomToRenderer(0);
	}

};

