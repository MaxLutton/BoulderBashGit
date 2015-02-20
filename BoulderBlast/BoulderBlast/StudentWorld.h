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

	virtual int init();

	virtual int move();

	virtual void cleanUp();
	int loadLevelObject(unsigned int curLevel, int x, int y);
	StudentWorld* getStudentWorld(){ return this; }
	Actor* getActor(int x, int y); 
	void updateDisplayText();

private:
	std::vector<Actor*> m_actors;
	Player* m_player;
	int m_bonus;
};

#endif // STUDENTWORLD_H_
