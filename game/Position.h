#pragma once
struct Position
{
public:
	int xPos;
	int yPos;
	Position(int x, int y);
	Position();
	~Position();

	bool operator ==(const Position &other) const
	{
		if (xPos == other.xPos && yPos == other.yPos)
			return true;
		else
			return false;
	}
	
	bool operator !=(const Position &other) const
	{
		if (xPos != other.xPos || yPos != other.yPos)
			return true;
		else
			return false;
	}
};

