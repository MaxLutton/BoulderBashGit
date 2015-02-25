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

class Hole : public Actor
{
public:
	Hole(int x, int y, StudentWorld* world) :Actor(IID_HOLE, x, y, none, world){}
	virtual void doSomething(){}//holes cant do shit
};

class Jewel : public Actor
{
public:
	Jewel(int x, int y, StudentWorld* world) :Actor(IID_JEWEL, x, y, none, world){}
	virtual void doSomething();
};

class Exit :public Actor
{
public:
	Exit(int x, int y, StudentWorld* world);
	virtual void doSomething(){}
};
#endif 

