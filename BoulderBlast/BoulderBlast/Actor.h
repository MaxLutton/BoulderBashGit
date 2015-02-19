#include "GraphObject.h"
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
private:
	StudentWorld* m_world;
	bool m_isAlive;
};

class Player : public Actor
{
public:
	Player(int startX, int startY, StudentWorld* world);
	virtual void doSomething();
	virtual int getHealth(){ return m_hitPoints; }
	int getAmmo(){ return m_ammo; }
private:
	int m_ammo;
	int m_hitPoints;
};
class Wall : public Actor
{
public:
	Wall(int startX, int startY, StudentWorld* world);
	virtual void doSomething(){}
};
#endif 

