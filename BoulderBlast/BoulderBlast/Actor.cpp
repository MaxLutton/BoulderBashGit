#include "Actor.h"
#include "StudentWorld.h"
#include "Level.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, int startX, int startY, Direction startDirection, StudentWorld* world) : GraphObject(imageID, startX, startY, startDirection)
{
	m_world = world;
	m_isAlive = true;
	setVisible(true);
}

Player::Player(int startX, int startY, StudentWorld* world) : Actor(IID_PLAYER, startX, startY, right, world)
{
	m_ammo = 20;
	m_hitPoints = 20;
}

Wall::Wall(int startX, int startY, StudentWorld* world) : Actor(IID_WALL, startX, startY, none, world){}

Boulder::Boulder(int startX, int startY, StudentWorld* world) : Actor(IID_BOULDER, startX, startY, none, world)
{
	m_hitPoints = 10;
}

void Player::doSomething()
{
	int x;
	int col = getX();
	int row = getY();
	StudentWorld* world = getWorld();
	unsigned int lev = world->getLevel();
	if (world->getKey(x))
	{
		int l, r, d, u;
		switch (x) //try to do what the player wants
		{
		case KEY_PRESS_LEFT: 
			setDirection(left);
			l = whatsThere(col - 1, row); 
			if (l == 0)
				moveTo(col - 1, row);
			if (l == 2 && whatsThere(col - 2, row) == 0)
			{
				Actor* rock = world->getActor(col - 1, row);
				rock->moveTo(col - 2, row);
				moveTo(col - 1, row);
			}
				break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			r = whatsThere( col + 1, row);
			if (r == 0)
				moveTo(col + 1, row);
			if (r == 2 && whatsThere(col + 2, row) == 0) //can push boulder
			{
				Actor* rock = world->getActor(col + 1, row);
				rock->moveTo(col + 2, row);
				moveTo(col + 1, row);
			}
			break;
		case KEY_PRESS_DOWN:
			d = whatsThere(col, row - 1);
			if (d == 0)
				moveTo(col, row - 1);
			if (d == 2 && whatsThere(col, row - 2) == 0)
			{
				Actor* rock = world->getActor(col, row - 1);
				rock->moveTo(col, row - 2);
				moveTo(col, row - 1);
			}		
			break;
		case KEY_PRESS_UP:
			u = whatsThere(col, row + 1);
			if (u == 0)
				moveTo(col, row + 1);
			if (u == 2 && whatsThere(col, row + 2) == 0)
			{
				Actor* rock = world->getActor(col, row + 1);
				rock->moveTo(col, row + 2);
				moveTo(col, row + 1);
			}
			break;
		case KEY_PRESS_SPACE: //if ammo isnt 0, shoot
			break; 
		case KEY_PRESS_ESCAPE:
			setIsAlive(false);
			//make dead
			//decrement lives, reset screen
			break;
		}
	}

}
int Actor::whatsThere(int x, int y)
{
	Actor* ap = m_world->getActor(x, y);
	if (ap != nullptr)
	{
		Wall* wp = dynamic_cast<Wall*>(ap);
		if (wp != nullptr)
			return 1;//wall
		Boulder* bp = dynamic_cast<Boulder*>(ap);
		if (bp != nullptr)
			return 2; //boulder
	}
	return 0; //space :D
}