#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

class Actor;
class Player;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), m_bonus(1000), m_actors(), m_player(nullptr)	{}
	virtual ~StudentWorld();

	virtual int init();

	virtual int move();

	virtual void cleanUp();
	int loadLevelObject(unsigned int curLevel, int x, int y);
	StudentWorld* getStudentWorld(){ return this; }
	std::vector<Actor*>* getm_Actors(); //returns pointer to m_Actors
	Player* getPlayer(); //returns pointer to m_player
	Actor* getActor(int x, int y); 
	void updateDisplayText();
	void decrJewels(); //decriments m_nJewels by one
	bool jewelsLeft(){ return m_nJewels > 0; } //returns if there are still jewels to be had

private:
	std::vector<Actor*> m_actors;
	Player* m_player;
	int m_bonus;
	int m_nJewels; //stores number of jewels left in level
};

inline Player* StudentWorld::getPlayer()
{ 
	return m_player; 
}

#endif // STUDENTWORLD_H_
