#include "GraphObject.h"
#include "GameConstants.h"
#ifndef Actor_h
#define Actor_h

class StudentWorld;

class Actor: public GraphObject
{
public:
	//constructor
	Actor(int imageID, int startX, int startY, Direction startDirection, StudentWorld* world);
	//returns a pointer to the StudentWorld variable
	StudentWorld* getWorld() const { return m_world; }
	//pure virtual function that must be implemented for all classes that will actually be allocated into the game
	//this function is called for every actor during every tick by StudentWorld's move(). 
	virtual void doSomething() = 0;
	//returns life status of actor. Used by StudentWorld's move() function to know which actors to delete from m_actors vector.
	//This makes Actor (and other classes without doSomething implementations) an abstract class.
	bool isAlive() { return m_isAlive; }
	//used to set actors' m_isAlive when an actor should die
	void setIsAlive(bool toWhat){ m_isAlive = toWhat; }
	//this returns an integer referring to the actor type that has the given x and y values. If there is nothing there, or if 
	//the only actor at that coordinate is a bullet or gem or goodie, then it returns 0. 
	int whatsThere(int x, int y);
	//This decrements the health of actors that have a m_hitPoints data member. It is virtual because not all actors 
	//have hit points. If it were called on an actor without health points, then this function does nothing.
	virtual void decHealth(){}
	//This returns the number of hit points that an actor has, if it has hitpoints. It is virtual because, for actors with hit points,
	//it actually has to do something. For actors without hit points, it just returns -1, which is meaningless.
	virtual double getHealth() { return -1; }
	//Empty virtual destructor. Although it doesn't do anything, it is necessary for all of my derived classes to 
	//be able to perform tasks during destruction
	virtual ~Actor(){}
	//This function is needed for KleptoBots. Doesn't do anything for other Actors, so just virtual.
	virtual void setHasGoodie(std::string type){}
	//This function is needed for KleptoBots. Just returns false for other Actors, so virtual.
	virtual bool holdingGoodie(){ return false; }
	//Used by bullets or jewels or goodies to determine if the player is currently on the same square as them.
	bool playerOnMe(int x, int y);
private:
	//a pointer to the StudentWorld object
	StudentWorld* m_world;
	//used to keep track of alive and dead Actors
	bool m_isAlive;
};

class Wall : public Actor
{
public:
	//constructor
	Wall(int startX, int startY, StudentWorld* world) : Actor(IID_WALL, startX, startY, none, world){}
	//Does nothing because walls don't do much. They aren't abstact, though.
	virtual void doSomething(){}
};

class Bullet : public Actor
{
public:
	//constructor
	Bullet(int startX, int startY, Direction dir, StudentWorld* world);
	//
	virtual void doSomething();
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

