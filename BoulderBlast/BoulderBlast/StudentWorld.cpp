#include "StudentWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

#include "StudentWorld.h"
//#include "Levels.h"


int StudentWorld::init()
{
	//push actors from gameworld into vector
	//load level file
	unsigned int lev = getLevel();
	for (int x = 0; x < VIEW_WIDTH; x++)
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
		int a = levelThings(lev, x, y);
		if (a == 1) //player
			m_player = new Player(x, y, this);
		if (a == 2)
			m_actors.push_back(new Wall(x, y, this));
		if (a == -1)
			return GWSTATUS_LEVEL_ERROR;
		}
	return GWSTATUS_CONTINUE_GAME;

}
int StudentWorld::move()
{
	updateDisplayText();
	std::vector<Actor*>::iterator it;
	it = m_actors.begin();
	//if actors aren't dead, have it do its thing
	while (it != m_actors.end())
	{
		if ((*it)->isAlive())
			(*it)->doSomething();
		it++;
	}
	m_player->doSomething();
	//played died!
	if (!(m_player->isAlive()))
	{	
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	it = m_actors.begin();
	while (it != m_actors.end()) //remove dead things
	{
		if (!(*it)->isAlive())
		{
			std::vector<Actor*>::iterator temp = it;
			it--;
			m_actors.erase(temp);
			it++;
		}
		else
			it++;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	std::vector<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end())
	{
		std::vector<Actor*>::iterator temp = it;
		it--;
		m_actors.erase(temp);
		it++;
	}
	delete m_player;
}

void StudentWorld::updateDisplayText()
{
/*Score: 0321000  Level : 05  Lives : 3  Health : 70 % Ammo : 20  Bonus : 742*/
	string a = "Score: " + getScore();
	string b = "  Level : " + getLevel();
	string c = "  Lives : " + getLives();
	string d = "  Health : " + m_player->getHealth();
	string e = " % Ammo : " + m_player->getAmmo();
	string f = "  Bonus : " + m_bonus;
	setGameStatText(a + b + c + d + e + f);	
}

int StudentWorld::levelThings(unsigned int curLevel, int x, int y)
{

		/*curLevel = "level03.dat";*/
	string a;
	if (curLevel == 0)
		a = "level00";
	else if (curLevel < 10)
		a = "level0" + curLevel;
	else
		a = "level" + curLevel;
	string theLevel = a + ".dat";
	Level	lev(assetDirectory());
	Level::LoadResult result = lev.loadLevel(theLevel);
	if (result == Level::load_fail_file_not_found ||
		result == Level::load_fail_bad_format)
		return -1; //	something	bad	happened!
	//	otherwise	the	load	was	successful	and	you	can	access	the
	//	contents	of	the	level – here’s	an	example	
	Level::MazeEntry item = lev.getContentsOf(x, y);
	if (item == Level::player)
		return 1;
	if (item == Level::wall)
		return 2;
	if (item == Level::boulder)
		return 3;
	else //will be for blank spaces later
		return 0;
}