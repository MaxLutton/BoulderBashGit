#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>


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
		Wall* wp = dynamic_cast<Wall*>(ap);
		if (wp != nullptr)
			return 2;//wall
		Boulder* bp = dynamic_cast<Boulder*>(ap);
		if (bp != nullptr)
			return 3; //boulder
		Hole* hp = dynamic_cast<Hole*>(ap);
		if (hp != nullptr)
			return 4; //hole
		KleptoBot* kp = dynamic_cast<KleptoBot*>(ap);
		if (kp != nullptr) //kleptobot
			return 5;
		KleptoBotFactory* fp = dynamic_cast<KleptoBotFactory*>(ap);
		if(fp != nullptr)
			return 6;
		SnarlBot* sp = dynamic_cast<SnarlBot*>(ap);
		if (sp != nullptr)
			return 7;
	}
	return 0; //space or bullet :D
}

void healthyActor::RestoreHealth()
{
	m_hitPoints = 20;
}

Player::Player(int startX, int startY, StudentWorld* world) : healthyActor(IID_PLAYER, startX, startY, right, world, 20)
{
	m_ammo = 20;
}

void Player::incrAmmo()
{
	m_ammo += 20;
}

void Player::decHealth()
{
	healthyActor::decHealth();
	getWorld()->playSound(SOUND_PLAYER_IMPACT);
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

Robot::Robot(int x, int y, Direction dir, StudentWorld* world, int ID, int hp) : healthyActor(ID, x, y, dir, world, hp)
{
	ticks = (28 - (world->levelNumber())) / 4;
	if (ticks < 3)
		ticks = 3;
}

void Robot::resetTicks()
{
	ticks = (28 - (getWorld()->levelNumber())) / 4;
	if (ticks < 3)
		ticks = 3;
}

bool Robot::shoot()
{
	Direction d = getDirection();
	int px = getWorld()->getPlayer()->getX();
	int py = getWorld()->getPlayer()->getY();
	int x = getX();
	int y = getY();
	StudentWorld* world = getWorld();
	if (d == left && y == py && x > px && shotNotBlocked(d))
	{
		world->getm_Actors()->push_back(new Bullet(x, y, d, world));
		world->playSound(SOUND_ENEMY_FIRE);
		return true;
	}
	else if (d == right && y == py && x < px && shotNotBlocked(d))
	{
		world->getm_Actors()->push_back(new Bullet(x, y, d, world));
		world->playSound(SOUND_ENEMY_FIRE);
		return true;
	}
	else if (d == up && x == px && y < py && shotNotBlocked(d))
	{
		world->getm_Actors()->push_back(new Bullet(x, y, d, world));
		world->playSound(SOUND_ENEMY_FIRE);
		return true;
	}
	else if (d == down && x == px && y > py && shotNotBlocked(d))
	{
		world->getm_Actors()->push_back(new Bullet(x, y, d, world));
		world->playSound(SOUND_ENEMY_FIRE);
		return true;
	}
	return false;
}

bool Robot::shotNotBlocked(Direction d)
{
	int x = getX();
	int y = getY();
	int px = getWorld()->getPlayer()->getX();
	int py = getWorld()->getPlayer()->getY();
	switch (d)
	{
	case right:
		while (x <= px)
		{
			if (playerOnMe(x,y))
				return true;
			else if (whatsThere(x + 1, y) != 0)
				return false;
				x++;
		}
		break;
	case left:
		while (x >= px)
		{
			if (playerOnMe(x,y))
				return true;
			else if (whatsThere(x - 1, y) != 0)
				return false;
			x--;
		}
		break;
	case down:
		while (y <= py)
		{
			if (playerOnMe(x,y))
				return true;
			else if (whatsThere(x, y + 1) != 0)
				return false;
			y++;
		}
		break;
	case up:
		while (y >= py)
		{
			if (playerOnMe(x,y))
				return true;
			else if (whatsThere(x, y - 1) != 0)
				return false;
			y--;
		}
	}
}

SnarlBot::SnarlBot(int x, int y, StudentWorld* world, Direction dir) : Robot(x, y, dir, world, IID_SNARLBOT, 10){}
SnarlBot::~SnarlBot()
{
	getWorld()->increaseScore(100);
}

void SnarlBot::doSomething()
{
	int t = getTicks();
	bool moved;
	//should rest
	if (t != 0)
	{
		setTicks(t - 1);
		return;
	}
	//try to shoot
	else if (shoot())
	{
		resetTicks();
		return;
	}
	//try to move
	else
	{
		//not resting
		resetTicks();
		bool moved = moveRobot();
	    //if cant move, just change direction
		if (moved == false)
		{
			Direction d = getDirection();
			switch (d)
			{
			case right:
				setDirection(left);
				break;
			case left:
				setDirection(right);
				break;
			case up:
				setDirection(down);
				break;
			case down:
				setDirection(up);
				break;
			}
		}

	}
}

Bullet::Bullet(int startX, int startY, Direction dir, StudentWorld* world) : Actor(IID_BULLET, startX, startY, dir, world){}

void Bullet::doSomething()
{
	//lolz stoopid idiot wrote the same code twice
	moveBullet();
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
		if (next == 2 || next == 6) //ran into wall
			setIsAlive(false);
		else if (next == 3 || next == 5 || next == 7) //hit a player or a boulder or bot
		{
			setIsAlive(false);
			Actor* guy = getWorld()->getActor(col - 1, row);
			guy->decHealth(); //bang bang you're dead
			if (guy->getHealth() <= 0)
				guy->setIsAlive(false);
		}
		else if (playerOnMe(col - 1, row))
		{
			Player* p = getWorld()->getPlayer();
			p->decHealth();
			setIsAlive(false);
			if (p->getHealth() <= 0)
				p->setIsAlive(false);
		}
			break;
	case right:
		next = whatsThere(col + 1, row);
		moveTo(col + 1, row);
		if (next == 2 || next == 6)
			setIsAlive(false);
		else if (next == 3 || next == 5 || next == 7)
		{
			setIsAlive(false);
			Actor* guy = getWorld()->getActor(col + 1, row);
			guy->decHealth();
			if (guy->getHealth() == 0) //just shot it dead
				guy->setIsAlive(false);
		}
		else if (playerOnMe(col + 1, row))
		{
			Player* p = getWorld()->getPlayer();
			setIsAlive(false);
			p->decHealth();
			if (p->getHealth() <= 0)
				p->setIsAlive(false);
		}
		break;
	case up:
		next = whatsThere(col, row + 1);
		moveTo(col, row + 1);
		if (next == 2 || next == 6)
			setIsAlive(false);
		else if (next == 3 || next == 5 || next == 7)
		{
			setIsAlive(false);
			Actor* guy = getWorld()->getActor(col, row+1);
			guy->decHealth();
			if (guy->getHealth() == 0) //just shot it dead
				guy->setIsAlive(false);
		}
		else if (playerOnMe(col, row + 1))
		{
			Player* p = getWorld()->getPlayer();
			setIsAlive(false);
			p->decHealth();
			if (p->getHealth() <= 0)
				p->setIsAlive(false);
		}
		break;
	case down:
		next = whatsThere(col, row - 1);
		moveTo(col, row - 1);
		if (next == 2 || next == 6 || next == 7)
			setIsAlive(false);
		else if (next == 3 || next == 5)
		{
			setIsAlive(false);
			Actor* guy = getWorld()->getActor(col, row - 1);
			guy->decHealth();
			if (guy->getHealth() == 0) //just shot it dead
				guy->setIsAlive(false);
		}
		else if (playerOnMe(col, row - 1))
		{
			Player* p = getWorld()->getPlayer();
			setIsAlive(false);
			p->decHealth();
			if (p->getHealth() <= 0)
				p->setIsAlive(false);
		}
		break;
	}
}


void Jewel::doSomething()
{
	int x = getX();
	int y = getY();
	int px = getWorld()->getPlayer()->getX();
	int py = getWorld()->getPlayer()->getY();
	//player on jewel, so give points, play sound, decrement m_nJewels member, and set this jewel to dead
	if (x== px && y == py)
	{
		getWorld()->increaseScore(50);
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->decrJewels();
		setIsAlive(false);
	}
}

Exit::Exit(int x, int y, StudentWorld* world) : Actor(IID_EXIT, x, y, none, world)
{
	setVisible(false);
}

void Exit::doSomething()
{
	int x = getX();
	int y = getY();
	StudentWorld* w = getWorld();
	//all jewels have been grabbed
	if (!w->jewelsLeft())
	{
		setVisible(true);
		int px = w->getPlayer()->getX();
		int py = w->getPlayer()->getY();
		if (px == x && py == y)
		{
			w->increaseScore(2000 + w->getBonus());
			w->playSound(SOUND_FINISHED_LEVEL);
			w->setLevelCompleted(true);
			setIsAlive(false);
		}
	}
}

bool Actor::playerOnMe(int x, int y)
{

	int px = getWorld()->getPlayer()->getX();
	int py = getWorld()->getPlayer()->getY();
	return (x == px && y == py);
}

void Goodie::doSomething()
{
	//kleptobot is on goodie
		if (whatsThere(getX(), getY()) == 5)
		{
			//one in ten chance of grabbing goodie
			Actor* klepto = getWorld()->getActor(getX(), getY());
			int r = rand() % 10;
			//can get the goodie if not already holding one
			if (r == 2 && !(klepto->holdingGoodie()))
			{
				std::string type;
				ExtraLifeGoodie* ep = dynamic_cast<ExtraLifeGoodie*>(this);
				if (ep != nullptr)
					type = "life";
				RestoreHealthGoodie* rp = dynamic_cast<RestoreHealthGoodie*>(this);
				if (rp != nullptr)
					type = "health";
				AmmoGoodie* ap = dynamic_cast<AmmoGoodie*>(this);
				if (ap != nullptr)
					type = "ammo";
				getWorld()->getActor(getX(), getY())->setHasGoodie(type);
				setIsAlive(false);
				getWorld()->playSound(SOUND_ROBOT_MUNCH);
			}
	}
}

void ExtraLifeGoodie::doSomething()
{
	StudentWorld* world = getWorld();
	if (playerOnMe(getX(), getY()))
	{
		world->increaseScore(1000);
		world->playSound(SOUND_GOT_GOODIE);
		setIsAlive(false);
		world->incLives();
	}
	else
		Goodie::doSomething();
}

void RestoreHealthGoodie::doSomething()
{
	StudentWorld* world = getWorld();
	if (playerOnMe(getX(), getY()))
	{
		world->increaseScore(500);
		world->playSound(SOUND_GOT_GOODIE);
		setIsAlive(false);
		world->getPlayer()->RestoreHealth();
	}
	else
		Goodie::doSomething();
}

void AmmoGoodie::doSomething()
{
	StudentWorld* world = getWorld();
	if (playerOnMe(getX(), getY()))
	{
		world->increaseScore(100);
		world->playSound(SOUND_GOT_GOODIE);
		setIsAlive(false);
		world->getPlayer()->incrAmmo();
	}
	else
		Goodie::doSomething();
}


void KleptoBotFactory::doSomething()
{
	StudentWorld* world = getWorld();
	int count = 0;
	int x = getX();
	int y = getY();
	//don't add a kleptoBot if there is one on the factory's square
	if (whatsThere(x, y) == 5)
		return;
	//can I make this more efficient? Is pretty awful right now...
	for (int c = 1; c < 4; c++)
		for (int r = 1; r < 4; r++)
		{
		if (x - c >= 0)
		{
			if (whatsThere(x - c, y) == 5)
				count++;
			if (y - r >= 0)
				if (whatsThere(x - c, y - r) == 5)
					count++;
		}
		if (x + c < VIEW_WIDTH)
		{
			if (whatsThere(x + c, y) == 5)
				count++;
			if (y + r < VIEW_HEIGHT)
				if (whatsThere(x + c, y + r) == 5)
					count++;
		}
		if (y - r >= 0)
		{
			if (whatsThere(x, y - r) == 5)
				count++;
			if (x + c < VIEW_WIDTH)
				if (whatsThere(x + c, y - r) == 5)
					count++;
		}
		if (y + r < VIEW_HEIGHT)
		{
			if (whatsThere(x, y + r) == 5)
				count++;
			if (x - c >= 0)
				if (whatsThere(x - c, y + r) == 5)
					count++;
		}
		}
	if (count < 3)
	{
		//1 in 50 chance of making new kleptobot
		int r = rand() % 50;
		if (r == 42 && m_angry == false)
		{
			world->getm_Actors()->push_back(new KleptoBot(getX(), getY(), world, IID_KLEPTOBOT, 5));
			world->playSound(SOUND_ROBOT_BORN);
		}//this is an angry kleptobot factory
		else if (r == 42 && m_angry == true)
		{
			world->getm_Actors()->push_back(new AngryKleptoBot(getX(), getY(), world));
			world->playSound(SOUND_ROBOT_BORN);
		}
	}
}

//Tries to move KleptoBot in its direction. If its direction is blocked, then this returns false. Otherwise, returns true.
bool Robot::moveRobot()
{
	int next = 0;
	int col = getX();
	int row = getY();
	Direction d = getDirection();
	Player* p = getWorld()->getPlayer();
	int px = p->getX();
	int py = p->getY();
	switch (d)
	{
	case left:
		next = whatsThere(col - 1, row);
		if (next == 0 && !playerOnMe(col - 1, row))
			moveTo(col - 1, row);
		else
			return false;
		break;
	case right:
		next = whatsThere(col + 1, row);
		if (next == 0 && !playerOnMe(col + 1, row))
			moveTo(col + 1, row);
		else
			return false;
		break;
	case up:
		next = whatsThere(col, row + 1);
		if (next == 0 && !playerOnMe(col, row + 1))
			moveTo(col, row + 1);
		else
			return false;
		break;
	case down:
		next = whatsThere(col, row - 1);
		if (next == 0 && !playerOnMe(col, row - 1))
			moveTo(col, row - 1);
		else
			return false;
		break;
	}
	return true;
}

KleptoBot::KleptoBot(int x, int y, StudentWorld* world, int ID, int hp) : Robot(x, y, right, world, ID, hp), hasGoodie(false)
{
	distanceBeforeTurning = rand() % 7 + 1;
}

AngryKleptoBot::AngryKleptoBot(int x, int y, StudentWorld* world) : KleptoBot(x, y, world, IID_ANGRY_KLEPTOBOT, 8){}

KleptoBot::~KleptoBot()
{
	if (hasGoodie)
	{
		if (typeOfGoodie == "life")
			getWorld()->getm_Actors()->push_back(new ExtraLifeGoodie(getX(), getY(), getWorld()));
		if (typeOfGoodie == "health")
			getWorld()->getm_Actors()->push_back(new RestoreHealthGoodie(getX(), getY(), getWorld()));
		if (typeOfGoodie == "ammo")
			getWorld()->getm_Actors()->push_back(new AmmoGoodie(getX(), getY(), getWorld()));
	}
	getWorld()->increaseScore(10);
}

AngryKleptoBot::~AngryKleptoBot()
{
	KleptoBot::~KleptoBot();
	getWorld()->increaseScore(10);
}

GraphObject::Direction getRandomDirection()
{
	int r = rand() % 4;
	switch (r)
	{
	case 0:
		return GraphObject::Direction::left;
		break;
	case 1:
		return GraphObject::Direction::right;
		break;
	case 2:
		return GraphObject::Direction::up;
		break;
	case 3:
		return GraphObject::Direction::down;
			break;
	}
}

void KleptoBot::doSomething()
{
	int t = getTicks();
	//should rest
	if (t != 0)
	{
		setTicks(t - 1);
		return;
	}
	else
		moveKleptoBot();
}

void Robot::decHealth()
{
	StudentWorld* world = getWorld();
	healthyActor::decHealth();
	if (getHealth() > 0)
		world->playSound(SOUND_ROBOT_IMPACT);
	if (getHealth() <= 0)
	{
		world->playSound(SOUND_ROBOT_DIE);
		setIsAlive(false);
	}
}

bool KleptoBot::holdingGoodie()
{
	return hasGoodie;
}

void KleptoBot::setHasGoodie(std::string type)
{
	hasGoodie = true;
	typeOfGoodie = type;
}

void KleptoBot::moveKleptoBot()
{
	//reset ticks so bot will rest after this turn
	resetTicks();
	bool notBlocked = false;
	//hasnt gone full distance yet. Check if direction is blocked, if not, moves to adjacent position.
	//If path is blocked, set flag to false so we know to reset direction
	if (distanceBeforeTurning != 0)
	{
		//if it moved, then sets notBlocked to true.
		notBlocked = moveRobot();
		distanceBeforeTurning--;
	}
	//either couldn't move or fulfilled distanceBeforeTurning
	if (notBlocked == false)
	{
		//get random distances and directions
		distanceBeforeTurning = rand() % 6 + 1;
		Direction d = getRandomDirection();
		setDirection(d);
		int i = 0;
		//if random direction doesn't work, then try all other directions! (including the random one again because idk what the current d is)
		//IS THIS OK?
		while (i < 5)
		{
			if (moveRobot())
				break;
			else if (i == 0)
				setDirection(left);
			else if (i == 1)
				setDirection(right);
			else if (i == 2)
				setDirection(up);
			else if (i == 3)
				setDirection(down);
			i++;
		}
		//went thru all directions and couldn't move, so set direction back to d
		if (i == 5)
			setDirection(d);
	}
}

void AngryKleptoBot::doSomething()
{
	int t = getTicks();
	//should rest
	if (t != 0)
	{
		setTicks(t - 1);
		return;
	}
	else if (shoot())
	{
		resetTicks();
		return;
	}
	else
		moveKleptoBot();
}