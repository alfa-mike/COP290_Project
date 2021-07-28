#include "GameObject.h"

GameObject::GameObject(std::shared_ptr<Room> room) :
	curObjRoom(room),
	objRenderer(room->roomRenderer),
	objPos(room->roomPos)
{
	SetObjectRect(&room->roomRect);
}

GameObject::GameObject(const GameObject &other): 
	objRect(other.objRect),
	objRenderer(other.objRenderer),
	objPos(other.objPos),
	curObjRoom(other.curObjRoom)
{	
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}
