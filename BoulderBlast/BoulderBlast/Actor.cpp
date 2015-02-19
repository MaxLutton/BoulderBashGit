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

void Player::doSomething()
{
	int x;
	int col = getX();
	int row = getY();
	StudentWorld* world = getWorld();
	unsigned int lev = world->getLevel();
	if (world->getKey(x))
	{
		switch (x) //FILL IN! 
		{
		case KEY_PRESS_LEFT: 
			setDirection(left); //need to check if direction is already left?
			if (world->levelThings(lev, col - 1, row) == 0)
				moveTo(col - 1, row);
			
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (world->levelThings(lev, col + 1, row) == 0)
				moveTo(col + 1, row);
			break;
		case KEY_PRESS_DOWN:
			if (world->levelThings(lev, col, row - 1) == 0)
				moveTo(col, row - 1);
			break;
		case KEY_PRESS_UP:
			if (world->levelThings(lev, col, row + 1) == 0)
				moveTo(col, row + 1);
			break;
		case KEY_PRESS_SPACE: //if ammo isnt 0, shoot
		case KEY_PRESS_ESCAPE:
			setIsAlive(false);
			//make dead
			//decrement lives, reset screen
			break;
		}
	}

}
Wall::Wall(int startX, int startY, StudentWorld* world) : Actor(IID_WALL, startX, startY, none, world){}