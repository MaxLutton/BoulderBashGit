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
int Actor::whatsThere(int x, int y)
{
	Actor* ap = m_world->getActor(x, y);
	if (ap != nullptr)
	{
		Player* pp = dynamic_cast<Player*>(ap);
		if (pp != nullptr)
			return 1; //player
		Wall* wp = dynamic_cast<Wall*>(ap);
		if (wp != nullptr)
			return 2;//wall
		Boulder* bp = dynamic_cast<Boulder*>(ap);
		if (bp != nullptr)
			return 3; //boulder
		Hole* hp = dynamic_cast<Hole*>(ap);
		if (hp != nullptr)
			return 4; //hole
	}
	return 0; //space or bullet :D
}

Player::Player(int startX, int startY, StudentWorld* world) : healthyActor(IID_PLAYER, startX, startY, right, world, 20)
{
	m_ammo = 20;
}

void Player::doSomething()
{
	int x;
	int col = getX();
	int row = getY();
	StudentWorld* world = getWorld();
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
			if (l == 3)
			{
				Actor* rock = world->getActor(col - 1, row);
				if (whatsThere(col - 2, row) == 0)
				{
					rock->moveTo(col - 2, row);
					moveTo(col - 1, row);
				}
				if (whatsThere(col - 2, row) == 4)
				{
					moveTo(col - 1, row);
					Actor* missingDirt = world->getActor(col - 2, row);
					missingDirt->setIsAlive(false);
					rock->setIsAlive(false);
				}
			}
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			r = whatsThere(col + 1, row);
			if (r == 0)
				moveTo(col + 1, row);
			if (r == 3)  //can push boulder
			{
				Actor* rock = world->getActor(col + 1, row);
				if (whatsThere(col + 2, row) == 0)
				{
					rock->moveTo(col + 2, row);
					moveTo(col + 1, row);
				}
				if (whatsThere(col + 2, row) == 4)//push rock into hole
				{
					Actor* missingDirt = world->getActor(col + 2, row);
					missingDirt->setIsAlive(false);
					rock->setIsAlive(false);
					moveTo(col + 1, row);
				}
			}
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			d = whatsThere(col, row - 1);
			if (d == 0)
				moveTo(col, row - 1);
			if (d == 3)
			{
				Actor* rock = world->getActor(col, row - 1);
				if (whatsThere(col, row - 2) == 0)//pushing boulder to empty
				{
					rock->moveTo(col, row - 2);
					moveTo(col, row - 1);
				}
				if (whatsThere(col, row - 2) == 4)//pushing boulder into hole
				{
					Actor* missingDirt = world->getActor(col, row - 2);
					missingDirt->setIsAlive(false);
					rock->setIsAlive(false);
					moveTo(col, row - 1);
				}
			}
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			u = whatsThere(col, row + 1);
			if (u == 0)
				moveTo(col, row + 1);
			if (u == 3)
			{
				Actor* rock = world->getActor(col, row + 1);
				if (whatsThere(col, row + 2) == 0) //pushing boulder to empty space
				{
					rock->moveTo(col, row + 2);
					moveTo(col, row + 1);//move player
				}
				if (whatsThere(col, row + 2) == 4)//pushing boulder into hole
				{
					Actor* missingDirt = world->getActor(col, row + 2);
					missingDirt->setIsAlive(false);
					rock->setIsAlive(false);
					moveTo(col, row + 1);//move player
				}
			}
			break;
		case KEY_PRESS_SPACE: //if ammo isnt 0, shoot
			if (m_ammo != 0)
			{
				getWorld()->getm_Actors()->push_back(new Bullet(col, row, getDirection(), getWorld()));
				m_ammo--;
				world->playSound(SOUND_PLAYER_FIRE);
			}
			break;
		case KEY_PRESS_ESCAPE: //make dead
			setIsAlive(false);
			break;
		}
	}
}



Bullet::Bullet(int startX, int startY, Direction dir, StudentWorld* world) : Actor(IID_BULLET, startX, startY, dir, world){}

void Bullet::doSomething()
{
	Direction x = getDirection();
	int col = getX();
	int row = getY();
	int here = whatsThere(col, row);
	int next;
	if (here == 0 || here == 4) //starts on free space (or bullet) or hole
		moveBullet();
	if (here == 1 || here == 3) //boulder or player
	{
		setIsAlive(false);
		getWorld()->getActor(col, row)->decHealth();
	}
}
//if bullet starts on a free space, then see what is on its adjacent space, then move bullet there. If adjacent space isnt empty, kill bullet/
//If item in adjacent space is a healthyActor, then decrement that actors health. 
void Bullet::moveBullet()
{
	Direction x = getDirection();
	int col = getX();
	int row = getY();
	int next;
	switch (x)
	{
	case left:
		next = whatsThere(col - 1, row);
		moveTo(col - 1, row);
		if (next == 2) //ran into wall
			setIsAlive(false);
		if (next == 1 || next == 3) //hit a player or a boulder
		{
			setIsAlive(false);
			Actor* guy = getWorld()->getActor(col - 1, row);
			guy->decHealth(); //bang bang you're dead
			if (guy->getHealth() == 0)
				guy->setIsAlive(false);
		}
			break;
	case right:
		next = whatsThere(col + 1, row);
		moveTo(col + 1, row);
		if (next == 2)
			setIsAlive(false);
		if (next == 1 || next == 3)
		{
			setIsAlive(false);
			Actor* guy = getWorld()->getActor(col + 1, row);
			guy->decHealth();
			if (guy->getHealth() == 0) //just shot it dead
				guy->setIsAlive(false);
		}
		break;
	case up:
		next = whatsThere(col, row + 1);
		moveTo(col, row + 1);
		if (next == 2)
			setIsAlive(false);
		if (next == 1 || next == 3)
		{
			setIsAlive(false);
			Actor* guy = getWorld()->getActor(col, row+1);
			guy->decHealth();
			if (guy->getHealth() == 0) //just shot it dead
				guy->setIsAlive(false);
		}
		break;
	case down:
		next = whatsThere(col, row -1);
		moveTo(col, row - 1);
		if (next == 2)
			setIsAlive(false);
		if (next == 1 || next == 3)
		{
			setIsAlive(false);
			Actor* guy = getWorld()->getActor(col, row - 1);
			guy->decHealth();
			if (guy->getHealth() == 0) //just shot it dead
				guy->setIsAlive(false);
		}
		break;
	}
}

