#include "GraphObject.h"
#include "GameConstants.h"
#ifndef Actor_h
#define Actor_h

class StudentWorld;

class Actor: public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction startDirection, StudentWorld* world);
	StudentWorld* getWorld() const { return m_world; }
	virtual void doSomething() = 0;
	bool isAlive() { return m_isAlive; }
	void setIsAlive(bool toWhat){ m_isAlive = toWhat; }
	int whatsThere(int x, int y);
	virtual void decHealth(){} //does nothin since no health
	virtual double getHealth() { return -1; }//does nothin since no health
	virtual ~Actor(){}//empty virtual destructor
	virtual void setHasGoodie(std::string type){}//needed for kleptobots
	virtual bool holdingGoodie(){ return false; }
	bool playerOnMe(int x, int y);//used by bullets or pickupable items to see if on player
private:
	StudentWorld* m_world;
	bool m_isAlive;
};

class Wall : public Actor
{
public:
	Wall(int startX, int startY, StudentWorld* world) : Actor(IID_WALL, startX, startY, none, world){}
	virtual void doSomething(){}
	virtual ~Wall(){}//empty virtual destructor
};

class Bullet : public Actor
{
public:
	Bullet(int startX, int startY, Direction dir, StudentWorld* world);
	virtual void doSomething();
	virtual ~Bullet(){}
	void moveBullet();
};

class healthyActor : public Actor
{
public:
	healthyActor(int imageID, int startX, int startY, Direction dir, StudentWorld* world, int hitpoints) : Actor(imageID, startX, startY, dir, world){ m_hitPoints = hitpoints; }
	virtual void doSomething() = 0;
	double getHealth() { return m_hitPoints; }
	virtual void decHealth(){ m_hitPoints -= 2; }
	virtual ~healthyActor(){}
	void RestoreHealth();
private:
	double m_hitPoints;
};

class Player : public healthyActor
{
public:
	Player(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
	int getAmmo(){ return m_ammo; }
	virtual ~Player(){}//empty virtual destructor
	void incrAmmo();
	virtual void decHealth();
private:
	int m_ammo;
};

class Boulder : public healthyActor
{
public:
	Boulder(int startX, int startY, StudentWorld* world) : healthyActor(IID_BOULDER, startX, startY, none, world, 10){}
	virtual void doSomething(){};
	virtual ~Boulder(){}//empty virtual destructor
};

class KleptoBotFactory : public Actor
{
public:
	KleptoBotFactory(bool angry, int x, int y, StudentWorld* world) : Actor(IID_ROBOT_FACTORY, x, y, none, world), m_angry(angry) {}
	virtual void doSomething();
	bool isRobotInArea(Actor* act);
	bool tooManyKleptos(int x, int y);
private:
	bool m_angry; //type of factory
};

//abstract class
class Robot : public healthyActor
{
public:
	Robot(int x, int y, Direction dir, StudentWorld* world, int ID, int hp);
	bool moveRobot();
	int getTicks() { return ticks; }
	void setTicks(int to) { ticks = to; }
	void resetTicks();
	bool shoot();
	virtual void decHealth();
	bool shotNotBlocked(Direction d);
private:
	int ticks;
};

class KleptoBot : public Robot
{
public:
	KleptoBot(int x, int y, StudentWorld* world, int ID, int hp);
	virtual void doSomething();
	void setHasGoodie(int type);
	bool holdingGoodie();
	void moveKleptoBot();
	virtual ~KleptoBot();
	bool grabGoodie();
private:
	int distanceBeforeTurning;
	bool hasGoodie;
	int typeOfGoodie;
};


class AngryKleptoBot :public KleptoBot
{
public:
	AngryKleptoBot(int x, int y, StudentWorld* world);
	virtual void doSomething();
	virtual ~AngryKleptoBot();
};

class SnarlBot : public Robot
{
public:
	SnarlBot(int x, int y, StudentWorld* world, Direction dir);
	virtual void doSomething();
	virtual ~SnarlBot();
};


class Hole : public Actor
{
public:
	Hole(int x, int y, StudentWorld* world) :Actor(IID_HOLE, x, y, none, world){}
	virtual void doSomething(){}//holes cant do shit
};
class Exit :public Actor
{
public:
	Exit(int x, int y, StudentWorld* world);
	virtual void doSomething();
};

//abstract class
class PickupableItem : public Actor
{
public:
	PickupableItem(int x, int y, StudentWorld* world, int ID) : Actor(ID, x, y, none, world){}
};
class Jewel : public PickupableItem
{
public:
	Jewel(int x, int y, StudentWorld* world) :PickupableItem(x, y, world, IID_JEWEL){}
	virtual void doSomething();
};

//abstract class
class Goodie : public PickupableItem
{
public:
	Goodie(int x, int y, StudentWorld* world, int ID) :PickupableItem(x, y, world, ID){}
};

class ExtraLifeGoodie :public Goodie
{
public:
	ExtraLifeGoodie(int x, int y, StudentWorld* world) : Goodie(x, y, world, IID_EXTRA_LIFE){}
	virtual void doSomething();
};

class RestoreHealthGoodie : public Goodie
{
public:
	RestoreHealthGoodie(int x, int y, StudentWorld* world) : Goodie(x, y, world, IID_RESTORE_HEALTH){}
	virtual void doSomething();
};

class AmmoGoodie : public Goodie
{
public:
	AmmoGoodie(int x, int y, StudentWorld* world) : Goodie(x, y, world, IID_AMMO){}
	virtual void doSomething();
};

#endif 

