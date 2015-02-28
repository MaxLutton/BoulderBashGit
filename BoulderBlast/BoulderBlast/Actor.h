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
	//if bullet starts on a free space, then see what is on its adjacent space, then move bullet there. If adjacent space isnt empty or something 
	//like a goodie kill bullet. If item in adjacent space is a healthyActor, then decrement that actors health and kill bullet. Play appropriate sound.
	virtual void doSomething();
};
//abstract class comprised of players, boulders, and robots
class healthyActor : public Actor
{
public:
	//constructor
	healthyActor(int imageID, int startX, int startY, Direction dir, StudentWorld* world, int hitpoints) : Actor(imageID, startX, startY, dir, world){ m_hitPoints = hitpoints; }
	//Purely virtual to make healthyActor an abstract class. Don't want to be making healthyActors accidentally.
	virtual void doSomething() = 0;
	//inherited from Actor so I could just use the getActor() function from StudentWorld. This function is actually used. It returns health. 
	virtual double getHealth() { return m_hitPoints; }
	//Again, was inherited from Actor class for use with polymorphism. When a healthyActor is hit by a bullet, this function is called.
	virtual void decHealth(){ m_hitPoints -= 2; }
	//Used by RestoreHealthGoodie to reset player's health points back to 20
	void RestoreHealth();
private:
	//data member that keeps track of healthyActor's hitpoints. 
	double m_hitPoints;
};

class Player : public healthyActor
{
public:
	//constructor
	Player(int startX, int startY, StudentWorld* world);
	//Depending on what key was inputted by the user, the player can move up, down, left, right, or shoot if it has ammo
	virtual void doSomething();
	//Used by doSomething function to determine if the player can shoot
	int getAmmo(){ return m_ammo; }
	//Used by AmmoGoodie to add 20 rounds to m_ammo
	void incrAmmo();
	//Inherited from healthyActor, but this plays the PLAYER_IMPACT sound when the function is called
	virtual void decHealth();
private:
	//only Players have a limited supply of ammo
	int m_ammo;
};

class Boulder : public healthyActor
{
public:
	//constructor
	Boulder(int startX, int startY, StudentWorld* world) : healthyActor(IID_BOULDER, startX, startY, none, world, 10){}
	//empty doSomething. Not an abstract class, but it doesn't do anything. Boulder movement was handled by Player's doSomething
	virtual void doSomething(){};
};

class KleptoBotFactory : public Actor
{
public:
	//Constructor. Requires parameter of angry to determine what type of kleptoBots the factory produces (either angry or not)
	KleptoBotFactory(bool angry, int x, int y, StudentWorld* world) : Actor(IID_ROBOT_FACTORY, x, y, none, world), m_angry(angry) {}
	//If there are fewer than 3 robots in area, has a 1 in 50 chance of creating its type of kleptoBot.
	virtual void doSomething();
	//Used by the tooManyKleptos function to determine if a given robot is within 3 squares left/right or up/down of the factory
	bool isRobotInArea(Actor* act);
	//used by doSomething to determine if the factory should try to create a kleptobot
	bool tooManyKleptos(int x, int y);
private:
	bool m_angry; //type of factory
};

//abstract subclass of healthyActor: includes KleptoBots, AngryKleptoBots, and Snarlbots
class Robot : public healthyActor
{
public:
	//constructor
	Robot(int x, int y, Direction dir, StudentWorld* world, int ID, int hp);
	//checks if robot's movement is not impeded in current direction. If not impeded, then move one square in that direction. Else, 
	//select a new direction.
	bool moveRobot();
	//returns the current number of ticks, which when equal to 0, allows the robot to doSomething
	int getTicks() { return ticks; }
	//used in construction and by resetTicks to set the ticks data member
	void setTicks(int to) { ticks = to; }
	//after a robot does something, it must reset its ticks to its initial value
	void resetTicks();
	//Checks if the robot has a clear line of sight to Player. If it does, it fires and returns true. Otherwise, it returns false.
	bool shoot();
	//Inherited from healthyActor, this decrements health and plays the sound SOUND_ROBOT_IMPACT. If the robot has 0 health after
	//decrementing, sets the robot to dead and plays SOUND_ROBOT_DEATH
	virtual void decHealth();
	//used by the shoot function, this returns whether or not the robot is facing the Player and has a clear shot
	bool shotNotBlocked(Direction d);
private:
	//keeps track of how much longer a robot has to wait until it can do something
	int ticks;
};

class KleptoBot : public Robot
{
public:
	//constructor called by factories. Since angry kleptobots are derived from this class, ID and hp are determined within factory implementation
	KleptoBot(int x, int y, StudentWorld* world, int ID, int hp);
	//If bot is supposed to rest, do nothing besides --ticks. Otherwise, if it is on the same square as a goodie, has 1 in 10 chance of 
	//picking up the goodie. Otherwise, try to move.
	virtual void doSomething();
	//Used by grabGoodie function. This sets the boolean hasGoodie to true and sets typeOfGoodie to the global constant integer for the goodie type.
	void setHasGoodie(int type);
	//Used by grabGoodie, returns hasGoodie data member.
	bool holdingGoodie();
	//If distanceBeforeTurning is not 0, try to move in current direction. If that does not work, find random direction and try to 
	//move in this direction. If that does not work, try all other directions, and if it still cannot move, reset direction to previoulsy-determined
	//random direction. Generate random in for distanceBeforeTurning..
	void moveKleptoBot();
	//Virtual destructor that plays SOUND_ROBOT_DEATH upon death and, if the bot had picked up a goodie, dynamically allocate a goodie 
	//of the same type and place it at the position where the kleptoBot died. Reset ticks. Gives points.
	virtual ~KleptoBot();
	//used by doSomething, if the bot is on the same position as a goodie and has not yet picked up a goodie, 1 in 10 chance of picking it up.
	//If it does pick it up, see what type of goodie it is, munch it up, and reset ticks. 
	bool grabGoodie();
private:
	int distanceBeforeTurning;
	bool hasGoodie;
	int typeOfGoodie;
};

//derived from KleptoBot 
class AngryKleptoBot :public KleptoBot
{
public:
	//constructor called by factories
	AngryKleptoBot(int x, int y, StudentWorld* world);
	//Has most of the same functionality as KleptoBot::doSomething() except, before trying to move, it tries to shoot the 
	//player. Uses the shoot() function from Robot class.
	virtual void doSomething();
	//Does the same thing as ~KleptoBot except gives 10 more points to the player
	virtual ~AngryKleptoBot();
};
//derived from Robot class
class SnarlBot : public Robot
{
public:
	//constructor
	SnarlBot(int x, int y, StudentWorld* world, Direction dir);
	//If ticks != 0, rests. Else, try to shoot using the Robot::shoot() function. Otherwise, try to move in its current direction.
	//If it can't move in its current direction, swap directions (left/right or up/down)
	virtual void doSomething();
	//Plays sound and gives Player points
	virtual ~SnarlBot();
};


class Hole : public Actor
{
public:
	//constructor
	Hole(int x, int y, StudentWorld* world) :Actor(IID_HOLE, x, y, none, world){}
	//Hole isn't abstract, but doesn't have to do anything. This is taken care of by the Player's doSomething
	virtual void doSomething(){}
};

class Exit :public Actor
{
public:
	//constructor. This sets isVisible to false within its body.
	Exit(int x, int y, StudentWorld* world);
	//If there are jewels left, don't do anything. When the last jewel is consumed, set isVisible to true.
	//When it is visible and the player is on the same square, player has completed level. Play sound and award points.
	virtual void doSomething();
};

//abstract class : jewels, goodies
class PickupableItem : public Actor
{
public:
	//constructor
	PickupableItem(int x, int y, StudentWorld* world, int ID) : Actor(ID, x, y, none, world){}
};

class Jewel : public PickupableItem
{
public:
	//constructor
	Jewel(int x, int y, StudentWorld* world) :PickupableItem(x, y, world, IID_JEWEL){}
	//if player is on same square as jewel, give points and play sound. Adjust m_nJewels accordingly. Set isAlive to false.
	virtual void doSomething();
};

//abstract class
class Goodie : public PickupableItem
{
public:
	//constructor
	Goodie(int x, int y, StudentWorld* world, int ID) :PickupableItem(x, y, world, ID){}
};

class ExtraLifeGoodie :public Goodie
{
public:
	//constructor
	ExtraLifeGoodie(int x, int y, StudentWorld* world) : Goodie(x, y, world, IID_EXTRA_LIFE){}
	//if player is on same square, give points, increase lives by 1, and play sound. Set isAlive to false.
	virtual void doSomething();
};

class RestoreHealthGoodie : public Goodie
{
public:
	//constructor
	RestoreHealthGoodie(int x, int y, StudentWorld* world) : Goodie(x, y, world, IID_RESTORE_HEALTH){}
	//if player is on same square, give points, restore health by using restoreHealth function, and play sound. Set isAlive to false.
	virtual void doSomething();
};

class AmmoGoodie : public Goodie
{
public:
	//constructor
	AmmoGoodie(int x, int y, StudentWorld* world) : Goodie(x, y, world, IID_AMMO){}
	//if player is on the same square, give points, increase m_ammo of player by 20, and play sound. Set isAlive to false.
	virtual void doSomething();
};

#endif 

