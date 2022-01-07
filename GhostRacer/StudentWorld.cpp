#include "StudentWorld.h"
#include "GameConstants.h"
using namespace std;
#include <string>
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}
// Students: Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp
 // defines the type std::ostringstream
#include <sstream> 
#include <cmath>
#include <iostream>
StudentWorld::~StudentWorld() {
	cleanUp();
}
StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath)
{
	m_GhostRacer = nullptr;
	m_Ycoordinate = 0;
	score_remaining = 5000;
	m_souls2save = 0;
	
}

//a function to find a proper position to insert new zombie cabs
bool StudentWorld::addNewZombieCab(int num, int& vspeed, int& ycordinate, int& xc) {
	//identify which lane is aksing to check
	//store the left edge of that lane and right edge of that lane in local variables
	int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
	int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
	int left, right, center;
	if (num == 1) {
		left = LEFT_EDGE;
		right = (ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH / 3);
		center = ROAD_CENTER - ROAD_WIDTH / 3;
	}
	else if (num == 2) {
	
		left = LEFT_EDGE + ROAD_WIDTH / 3;
		right = RIGHT_EDGE - ROAD_WIDTH / 3;
		center = ROAD_CENTER;
	}
	else {
		left = RIGHT_EDGE - ROAD_WIDTH / 3;
		right = RIGHT_EDGE;
		center = ROAD_CENTER + ROAD_WIDTH / 3;
	}
	int yc = -1;//flag variable
	//Determine the closest “collision avoidance-worthy actor” to the BOTTOM
	//of the screen in candidate lane
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
		if ((*it)->getX() >= left && (*it)->getX() < right) {//check if it is inside the specific lane
			if ((*it)->collisionAvoidanceActor()) {//check if it is a collision Avoidance Actor
				if (yc != -1) {//check if there is already an object at that lane
					if (((*it)->getY()) < yc) {
						yc = (*it)->getY();
					}
				}
				else {
					yc = (*it)->getY();
				}
			}
		}
	}
	//check if the ghost racer is at that lane, then dont insert
	if (getGhostRacer()->getX() < left || getGhostRacer()->getX() >= right) {
		//check if it is a proper position
		if (yc == -1 || yc > (VIEW_HEIGHT / 3)) {
			//update the reference values, and return true
			xc = center;
			ycordinate = SPRITE_HEIGHT / 2;
			vspeed = getGhostRacer()->getYspeed() + randInt(2, 4);
			return true;
		}
	}
	int yx = -1;//flag variable
	//Determine the closest “collision avoidance-worthy actor” to the BOTTOM
	//of the screen in candidate lane
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
		if ((*it)->getX() >= left && (*it)->getX() < right) {//check if it is inside the specific lane
			if ((*it)->collisionAvoidanceActor()) {//check if it is a collision Avoidance Actor
				if (yx != -1) {
					if ((VIEW_HEIGHT - (*it)->getY()) < (VIEW_HEIGHT - yx)) {
						//get the closet “collision avoidance-worthy actor”'s y coordinate to the TOP
						yx = (*it)->getY();
					}
				}else {
					yx = (*it)->getY();
				}
			}
		}
	}
	//if this is a proper position, then updatet the reference value, and return true
	if (yx == -1 || yx < (VIEW_HEIGHT * 2 / 3)) {
		xc = center;
		ycordinate = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
		vspeed = getGhostRacer()->getYspeed() - randInt(2, 4);
		return true;
	}
	return false;

}

int StudentWorld::getSouls() const {
	return m_souls2save;
}

void StudentWorld::incSouls() {
	m_souls2save++;
}


int StudentWorld::init()
{
	//initialize everything to begin when a level up
	m_souls2save = 0;
	score_remaining = 5000;
	//yellow border line
	int N = VIEW_HEIGHT / SPRITE_HEIGHT;
	int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
	int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
	for (int j = 0; j < N; j++) {
		m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE, LEFT_EDGE, j
			* SPRITE_HEIGHT));
		m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE, RIGHT_EDGE,
			j * SPRITE_HEIGHT));
	}
	//white border line
	int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
	for (int j = 0; j < M; j++) {
		m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, LEFT_EDGE +
			ROAD_WIDTH / 3, j *
			(4 * SPRITE_HEIGHT)));
		m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, RIGHT_EDGE -
			ROAD_WIDTH / 3, j *
			(4 * SPRITE_HEIGHT)));
		m_Ycoordinate = j * (4 * SPRITE_HEIGHT);
	}
	//construct ghost racer
	m_GhostRacer = new GhostRacer(this);
	return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	//add text in by using stramstram
	ostringstream oss;
	int souls = 2 * getLevel() + 5;
	oss << "Score: " << getScore() << "  Lvl: " << getLevel() << "  Souls2Save: " << souls-getSouls()
		<< "  Lives: " << getLives() << "  Health: " << getGhostRacer()->getHP() << "  Sprays: " << getGhostRacer()->getNumSprays()
		<< "  Bonus: " << score_remaining;
	string s = oss.str();
	setGameStatText(s);


	//decrease the bonus points for each tick
	if (score_remaining <= 0) {
		score_remaining = 0;
	}
	else {
		score_remaining--;
	}

	//step 1
	//tell all of the actors that are currently active in the game world to do
	//something
		for (vector<Actor*>::iterator a = m_actors.begin(); a != m_actors.end(); a++) {
			if ((*a)->isnotDead()) {
				(*a)->doSomething();
			}
		}
	m_GhostRacer->doSomething();

	//step 3
	//adding new actors in the game

	//adding new white border lines
	int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;//step 1
	m_Ycoordinate = m_Ycoordinate - 4 - (m_GhostRacer->getYspeed());//update the m_Ycoordinate to the last white border line's y coordinate
	int delta_y = new_border_y - m_Ycoordinate;//step 2
	//add yellow border line
	if (delta_y >= SPRITE_HEIGHT) {
		m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE,
			(ROAD_CENTER - ROAD_WIDTH / 2), new_border_y));
		m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE,
			ROAD_CENTER + ROAD_WIDTH / 2, new_border_y));
	}
	//add white border line
	if (delta_y >= (SPRITE_HEIGHT * 4)) {
		m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, (ROAD_CENTER
			-ROAD_WIDTH / 2 +ROAD_WIDTH / 3), new_border_y));
		m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, (ROAD_CENTER+ ROAD_WIDTH / 2 -ROAD_WIDTH / 3), new_border_y));
		m_Ycoordinate = new_border_y;
	}
	int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
	int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;

	int L = getLevel();//store the game's level
	//add human ped
	int ChanceHumanPed = max(200 - L * 10, 30);
	int random = randInt(0, ChanceHumanPed - 1);
	if (random == 0) {
		m_actors.push_back(new HumanPedestrian(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
	}
	//add zombie ped
	int ChanceZombiePed = max(100 - L * 10, 20);
	random = randInt(0, ChanceZombiePed - 1);
	if (random == 0) {
		m_actors.push_back(new ZombiePedestrian(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
	}
	//add oil slick
	int ChanceOilSlick = max(500 - L * 10, 40);
	int random2 = randInt(0, ChanceOilSlick-1);
	if (random2 == 0) {
		m_actors.push_back(new OilSlick(this, randInt(LEFT_EDGE,
			RIGHT_EDGE), VIEW_HEIGHT));
	}
	//add Holy Water
	int ChanceOfHolyWater = 100 + 10 * L;
	random = randInt(0, ChanceOfHolyWater-1);
	if (random == 0) {
		m_actors.push_back(new HolyWaterGoodie(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));
	}
	//add lost soul
	int ChanceOfLostSoul = 100;
	random = randInt(0, ChanceOfLostSoul-1);
	if (random == 0) {
		m_actors.push_back(new SoulGoodie(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));
	}

	//add zombie cab
	int ChanceVehicle = max(100-L*10, 20);
	random = randInt(0, ChanceVehicle - 1);
	if (random == 0) {
		//the values will change after find proper position to insert the zombie cab
		int Verspeed=-999, yc=-999, xc=-999;
		bool insert=false;
		int cur_lane = randInt(1, 3);
		//equal probability in which to start our new zombie cab
		//for each value of cur lane, using addNewZombieCab() function to determine a proper position to add zombie cab
		if (cur_lane == 1) {
			insert = addNewZombieCab(1, Verspeed, yc, xc);
			if (insert == false) {
				insert = addNewZombieCab(2, Verspeed, yc, xc); 
					if (insert == false) {
						insert = addNewZombieCab(3, Verspeed, yc, xc);
					}
				}
		}
		///for each value of cur lane, using addNewZombieCab() function to determine a proper position to add zombie cab
		else if (cur_lane == 2) {
			insert = addNewZombieCab(2, Verspeed, yc, xc);
			if (insert == false) {
				insert = addNewZombieCab(3, Verspeed, yc, xc);
				if (insert == false) {
					insert = addNewZombieCab(1, Verspeed, yc, xc);
				}
			}
		}else {
			//for each value of cur lane, using addNewZombieCab() function to determine a proper position to add zombie cab
			insert = addNewZombieCab(3, Verspeed, yc, xc);
			if (insert == false) {
				insert = addNewZombieCab(1, Verspeed, yc, xc);
				if (insert == false) {
					insert = addNewZombieCab(2, Verspeed, yc, xc);
				}
			}
		}
		//if found a proper position to insert a zombie cab, then inserting a new zombie cab
		if (insert == true) {
			m_actors.push_back(new ZombieCab(this, xc, yc));
			vector<Actor*>::iterator ittozombie = m_actors.end() - 1;
			(*ittozombie)->setYSpeed(Verspeed);
		}
	}
	// step 1b
		//if Ghost Racer has saved the required number of souls for the
		//current level(for level L, that's L*2 + 5), then it’s time to advance to the
			//next level
	if (getSouls() ==2*getLevel()+5) {
		increaseScore(score_remaining);
		while (getLives() != 3) {
			incLives();
		}
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	//step 2
	//then delete any actors that have died during this tick
	for (vector<Actor*>::iterator it2 = m_actors.begin(); it2 != m_actors.end();) {
		if (!(*it2)->isnotDead()) {
			delete* it2;
			it2 = m_actors.erase(it2);
		}
		else {
			it2++;
		}
	}
	//step 1a
	//if ghost racer's HP is below or equal to 0, then decrease one liv
	//restart the level
	//if ghost racer's HP is below or equal to 0, then decrease one liv
	if (!getGhostRacer()->isnotDead()) {
		decLives();
		playSound(SOUND_PLAYER_DIE);
		return GWSTATUS_PLAYER_DIED;
	}


	return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp()
{
	//delete all the actors in the container
	vector<Actor*>::iterator it;
	for (it = m_actors.begin(); it != m_actors.end();) {
		delete* it;
		it = m_actors.erase(it);
	}
	//delete the ghost racer and set it to nullptr
	delete m_GhostRacer;
	m_GhostRacer = nullptr;
}

//check if the ghost racer is at a valid position
//change ghost racer's direction if hit the yellow border line
bool StudentWorld::validPos() {
	int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
	int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;

	//If the Ghost Racer's center X coordinate is less than or equal to the left road
	//boundary, then change direction
	if (m_GhostRacer->getX() <= LEFT_EDGE) {
		//decrease the HP, if angle>90
		if (m_GhostRacer->getDirection() > 90) {
			m_GhostRacer->setHP(-10);
		}
		m_GhostRacer->setDirection(82);
		playSound(SOUND_VEHICLE_CRASH);
		return true;
	}
	if (m_GhostRacer->getX() >= RIGHT_EDGE) {
		//decrease the HP, if angle < 90
		if (m_GhostRacer->getDirection() < 90) {
			m_GhostRacer->setHP(-10);
		}
		m_GhostRacer->setDirection(98);
		playSound(SOUND_VEHICLE_CRASH);
		return true;
	}
	return false;
}

//return a ptr to ghost racer
GhostRacer* StudentWorld::getGhostRacer() const {
	return m_GhostRacer;
}

//check if this actor is overlapped with ghost racer
GhostRacer* StudentWorld::getOverlappingGhostRacer(Actor* a) const {
	double delta_x = abs(a->getX() - getGhostRacer()->getX());
	double delta_y = abs(a->getY() - getGhostRacer()->getY());
	double radius_sum = a->getRadius() + getGhostRacer()->getRadius();
	if (delta_x < radius_sum * .25 && delta_y < radius_sum * .6) {
		return getGhostRacer();//return a pointer to ghost racer
	}
	return nullptr;//if these two actors are not overlapped with each other, than return false
}

//check if two actors are overlapped with each other
bool StudentWorld::overlaps(const Actor* a1, const Actor* a2) const {
	double delta_x = abs(a1->getX() - a2->getX());
	double delta_y = abs(a1->getY() - a2->getY());
	double radius_sum = a1->getRadius() + a2->getRadius();
	if (delta_x < radius_sum * .25 && delta_y < radius_sum * .6) {
		return true;//if overlapped, return true
	}
	return false;//otherwise return false
}

//return the Y coordinate of the closest collision-avoidance worthy actor in front of that zombie cab
int StudentWorld::aboveZombieCab(const Actor* a, bool isFront) {
	int left, right;
	int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
	int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;

	//find which length that this zombie cab is in
	if (a->getX() > LEFT_EDGE && a->getX() < LEFT_EDGE + ROAD_WIDTH / 3) {
		left = LEFT_EDGE;
		right = LEFT_EDGE +
			ROAD_WIDTH / 3;
	}
	else if (a->getX() > LEFT_EDGE + ROAD_WIDTH / 3 && a->getX() < RIGHT_EDGE - ROAD_WIDTH / 3) {
		left = LEFT_EDGE + ROAD_WIDTH / 3;
		right = RIGHT_EDGE - ROAD_WIDTH / 3;
	}
	else {
		left = RIGHT_EDGE - ROAD_WIDTH / 3;
		right = RIGHT_EDGE;
	}
	int Yc = -1;

	if (isFront == true) {
		//check if there is any actor in front of the zombie cab in that lane and set the y coordinate 
	//to the closest actor's y coordinate
		for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
			if ((*it)->collisionAvoidanceActor()&&(*it)!=a) {
				if ((*it)->getY() > a->getY()) {
					if ((*it)->getX() >= left && (*it)->getX() < right) {
						if (Yc != -1) {
							if ((*it)->getY() - a->getY() < Yc - a->getY()) {
								Yc = (*it)->getY();
							}
						}
						else {
							Yc = (*it)->getY();
						}
					}
				}
			}
		}
	}

		//add to check ghost racer if necessary
	else if (isFront == false) {
		//check if there is any actor behind of the zombie cab in that lane and set the y coordinate 
	//to the closest actor's y coordinate
		for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
			if ((*it)->collisionAvoidanceActor() && (*it) != a) {
				if ((*it)->getY() < a->getY()) {
					if ((*it)->getX() > left && (*it)->getX() < right) {
						if (Yc != -1) {
							if (a->getY() - (*it)->getY() < a->getY() - Yc) {
								Yc = (*it)->getY();
							}
						}
						else {
							Yc = (*it)->getY();
						}
					}
				}
			}
		}
	}
	return Yc;
	//if there is no such a actor in front of or behind the y coordinate, return -1, otherwise return the closest y coordinate
}

//if an actor is affected by the holy water
bool StudentWorld::affectedwithProjectile(Actor* a1) {
	//traverse through the container to find if holy water is overlapped with one actor that can be affected by holy water
	for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
		if ((*it)->beSprayedIfAppropriate()) {
			if (overlaps(a1, *it)) {
				//damage that actor in proper way and set the holy water to death
				(*it)->damage();
				a1->setDead();
				return true;
			}
		}
	}
	return false;
}

void StudentWorld::addActor(Actor* a) {
m_actors.push_back(a);
}
