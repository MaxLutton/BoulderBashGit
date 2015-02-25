#include "StudentWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

#include "StudentWorld.h"
//#include "Levels.h"

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir), m_bonus(1000), m_actors(), m_player(nullptr), m_levelCompleted(false)	{}

StudentWorld::~StudentWorld()
{
	delete m_player;
	std::vector<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end())
	{
		delete (*it);
		it = m_actors.erase(it);
	}

}

int StudentWorld::init()
{
	//push actors from gameworld into vector
	//load level file
	unsigned int lev = getLevel();
	for (int x = 0; x < VIEW_WIDTH; x++)
		for (int y = 0; y < VIEW_HEIGHT; y++)
		{
		int a = loadLevelObject(lev, x, y);
		if (a == 1) //player
			m_player = new Player(x, y, this);
		if (a == 2) //wall
			m_actors.push_back(new Wall(x, y, this));
		if (a == 3) //boulder
			m_actors.push_back(new Boulder(x, y, this));
		if (a == 4)//hole
			m_actors.push_back(new Hole(x, y, this));
		if (a == 5)//jewel
		{
			m_actors.push_back(new Jewel(x, y, this));
			m_nJewels++;
		}
		if (a == 6)// exit
			m_actors.push_back(new Exit(x, y, this));
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
			delete *temp;
			m_actors.erase(temp);
			it++;
		}
		else
			it++;
	}
	if (m_levelCompleted)
	{
		m_levelCompleted = false;
		return GWSTATUS_FINISHED_LEVEL;
	}
	if (m_bonus != 0)
		m_bonus--;
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	std::vector<Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end())
	{
		delete (*it);
		it = m_actors.erase(it);
	}
	delete m_player;
}

void StudentWorld::updateDisplayText()
{
/*Score: 0321000  Level : 05  Lives : 3  Health : 70 % Ammo : 20  Bonus : 742*/
	//string text = "Score: " + to_string(getScore()) + "  Level : " + to_string(getLevel()) + "  Lives : " + to_string(getLives()) +
	//	"  Health : " + to_string(m_player->getHealth()) + " % Ammo : " + to_string(m_player->getAmmo()) + "  Bonus : " + to_string(m_bonus);
	//setGameStatText(text);	
	ostringstream oss;
	oss.fill('0');
	oss << "Score: " << setw(7) << getScore();
	oss << " Level: " << setw(2) << getLevel();
	oss.fill(' ');
	oss << " Lives: " << setw(2) << getLives();
	int health = (m_player->getHealth()) / 20;
	health = health * 100;
	oss << " Health: " << setw(3) << health << "%";
	oss << " Ammo: " << setw(3) << m_player->getAmmo();
	oss << " Bonus: " << setw(4) << m_bonus;
	setGameStatText(oss.str());
	
}

int StudentWorld::loadLevelObject(unsigned int curLevel, int x, int y)
{

		/*curLevel = "level03.dat";*/
	string a;
	if (curLevel == 0)
		a = "level00";
	else if (curLevel < 10)
		a = "level0" + to_string(curLevel);
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
	if (item == Level::hole)
		return 4;
	if (item == Level::jewel)
		return 5;
	if (item == Level::exit)
		return 6;
	else //will be for blank spaces later
		return 0;
}
Actor* StudentWorld::getActor(int x, int y) //get pointer to actor at coordinate
{
	vector<Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end())
	{
		int col = (*it)->getX();
		int row = (*it)->getY();
		if (col == x && row == y)
		{	
			Jewel* jp = dynamic_cast<Jewel*>(*it);
			Exit* xp = dynamic_cast<Exit*>(*it);
			if (jp == nullptr && xp == nullptr)
				return *it; //not the jewel or exit
			it++; ///was a jewel or exit
		}
		else
			it++;
	}
	return nullptr; 
}
std::vector<Actor*>* StudentWorld::getm_Actors()
{ 
	std::vector<Actor*>* point = &m_actors;
	return point;
}
void StudentWorld::decrJewels()
{
	m_nJewels--;
}