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
	virtual int getHealth() { return -1; }//does nothin since no health
	virtual ~Actor(){}//empty virtual destructor
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
	int getHealth(){ return m_hitPoints; }
	void decHealth(){ m_hitPoints -= 2; }
	virtual ~healthyActor(){}
	void RestoreHealth();
private:
	int m_hitPoints;
};

class Player : public healthyActor
{
public:
	Player(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
	int getAmmo(){ return m_ammo; }
	virtual ~Player(){}//empty virtual destructor
	void incrAmmo();
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
private:
	bool m_angry; //type of factory
};

//abstract class
class Robot : public healthyActor
{
public:
	Robot(int x, int y, Direction dir, StudentWorld* world, int ID, int hp);
	int getTicks() { return ticks; }
	void setTicks(int to) { ticks = to; }
	void resetTicks();
private:
	int ticks;
};

class KleptoBot : public Robot
{
public:
	KleptoBot(int x, int y, StudentWorld* world);
	virtual void doSomething();
	void setHasGoodie();
	bool moveKlepto();
private:
	int distanceBeforeTurning;
	bool hasGoodie;
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
	bool IsPlayerOnMe(StudentWorld* world);//used by items to see if the player is at same coordinate as item
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

