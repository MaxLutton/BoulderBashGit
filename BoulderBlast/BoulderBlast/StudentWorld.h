#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

class Actor;
class Player;
class Goodie;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	//constructor. Sets bonus points to 1000, pointers to nullptr, and initializes m_actors as an empty vector.
	StudentWorld(std::string assetDir);
	//virtual destructor. Clears out all dynamically allocated elements of m_actors and destroys m_player
	virtual ~StudentWorld();
	//Loads level file's data and dynamically allocats Actors accordingly into the vector. Sets bonus to 1000 and m_nJewels to number
	//of Jewels that it added. Either returns to continue game or that there was an errror in the level.
	virtual int init();
	//Gives every Actor a chance to call its doSomething function, including the player. If the player died during this tick, 
	//decrement # of lives and return appropriate status. Remove all of the dead actors from the vector. Check to see if level was completed,
	//and if it was, return appropriate constant. Decrement bonus points, and continue game.
	virtual int move();
	//Destroy player and all elements of m_actors vector.
	virtual void cleanUp();
	//this is used by init to determine which types of Actors should be allocated with what coordinates
	int loadLevelObject(unsigned int curLevel, int x, int y);
	//return pointer to this studentworld to be used by the Actor implementations
	StudentWorld* getStudentWorld(){ return this; }
	//returns pointer to m_Actors vector
	std::vector<Actor*>* getm_Actors(); 
	//returns pointer to m_player
	Player* getPlayer(); 
	//searches through the m_actors vector and returns the first instance of an actor with the corresponding coordinates.
	//If no such actor exists, returns nullptr.This excludes pickupableitems and the exit
	Actor* getActor(int x, int y); 
	//returns m_bonus
	int getBonus();
	//gets the values necessary for the display text and uses setGameStatText
	void updateDisplayText();
	//decriments m_nJewels by one
	void decrJewels(); 
	//returns if there are still jewels to be had
	bool jewelsLeft();
	//used by exit to let move know that level has been completed
	void setLevelCompleted(bool to);
	//returns current level number
	int levelNumber();
	//used by KleptoBots to get a pointer to a goodie at the current coordinates. If none exists, returns nullptr
	Goodie* getGoodie(int x, int y);

private:
	std::vector<Actor*> m_actors;
	Player* m_player;
	int m_bonus;
	int m_nJewels; //stores number of jewels left in level
	bool m_levelCompleted;
};

inline  bool StudentWorld::jewelsLeft()
{
	return m_nJewels > 0;
}

inline int StudentWorld::levelNumber()
{ 
	return getLevel(); 
}

inline Player* StudentWorld::getPlayer()
{ 
	return m_player; 
}
inline int StudentWorld::getBonus()
{
	return m_bonus;
}

inline void StudentWorld::setLevelCompleted(bool to)
{
	m_levelCompleted = to;
}


#endif // STUDENTWORLD_H_
