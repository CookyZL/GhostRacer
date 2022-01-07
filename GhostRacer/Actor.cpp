#include "Actor.h"
#include "StudentWorld.h"
// Students: Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
//Actor
#include <cmath>
Actor::Actor(StudentWorld* studentWorld, int imageID, double startX, double startY, int dir,
	double size, unsigned int depth,int Yspeed) : GraphObject(imageID, startX,
		startY,
		dir, size, depth), m_studentWorld(studentWorld), m_alive(true),
	m_Yspeed(Yspeed)
{}
//destructor
Actor::~Actor() {}

//return a pointer to StudentWorld
StudentWorld* Actor::getWorld() {
	return m_studentWorld;
}

// Does this object affect zombie cab placement and speed?
bool Actor::collisionAvoidanceActor() const {
	return false;
}

// Set this actor's vertical speed
void Actor::setYSpeed(double num) {
	m_Yspeed = m_Yspeed + num;
}

// Is this actor dead?
bool Actor::isnotDead() const {
	return m_alive;
}
// Mark this actor as dead.
void Actor::setDead()
{
	m_alive = false;
}

// Get this actor's vertical speed.
double Actor::getYspeed() const {
	return m_Yspeed;
}

// If this actor is affected by holy water projectiles, then inflict that
	// affect on it and return true; otherwise, return false
bool Actor::beSprayedIfAppropriate() {
	return false;
}

// Adjust the x coordinate by dx to move to a position with a y coordinate
	  // determined by this actor's vertical speed relative to GhostRacser's
	  // vertical speed.  Return true if the new position is within the view;
	  // otherwise, return false, with the actor dead.
bool Actor::moveRelativeToGhostRacerVerticalSpeed(double dx) {
	double vert_speed = getYspeed() - getWorld()->getGhostRacer()->getYspeed();//define the new vertical speed
	double horiz_speed = dx;//get horizontal speed
	double new_y = this->getY() + vert_speed;
	double new_x = this->getX() + horiz_speed;
	moveTo(new_x, new_y);//update the new position
	//check if the actor is out of bound, if it is set it to dead 
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
		setDead();
		return false;
	}
	return true;
}

//Agent
//Agent
//initialize agent's vertical speed to 0
//remember to change the virtical speed for other actors
Agent::Agent(StudentWorld* sw, int imageID, double x, double y, double size, int dir, int hp)
	:Actor(sw,imageID,x,y,dir,size,0,0), m_hit(hp),x_speed(0), m_movementPlanDis(0){}

Agent::~Agent() {}

bool Agent::collisionAvoidanceActor() const {
	return true;
}

//get the plan distance
int Agent::getPlan() const {
	return m_movementPlanDis;
}

//decrement plan distance
void Agent::decreasePlan() {
	m_movementPlanDis--;
}

//set plan to a new number
void Agent::setPlan(int s) {
	m_movementPlanDis = s;
}

// Get hit points.
int Agent::getHP() const {
	return m_hit;
}

//except ghost racer, other agent derivived class has a virtual damaged function
//this function in agent do nothing
//only allowed the ghost racer class to initialize
void Agent::damage() {
}
// Do what the spec says happens when hp units of damage is inflicted.
	// Return true if this agent dies as a result, otherwise false.
bool Agent::takeDamageAndPossiblyDie(int hp) {
	m_hit = m_hit - hp;
	// reduced the hp and check if dead.
	if (m_hit <= 0) {
		setDead();
		return true;
	}
	return false;
}

// Increase hit points by hp.
void Agent::setHP(int hp) {
	//the max hit pts is 100
	if (m_hit + hp >= 100) {
		m_hit = 100;
	}
	else {
		m_hit = m_hit + hp;
	}
}

int Agent::soundWhenHurt() {
	return SOUND_VEHICLE_HURT;
}

//What sound should play when this agent is damaged and dies?
int Agent::soundWhenDie() {
	return SOUND_VEHICLE_DIE;
}
int Agent::getHorizSpeed() const {
	return x_speed;
}
//set the horizontal speed to the input speed
void Agent::setHorizSpeed(int s) {
	x_speed = s;
}
//Ghost Racer
GhostRacer::GhostRacer(StudentWorld* studentWorld)
	:Agent(studentWorld, IID_GHOST_RACER, 128, 32,4.0,90,100),m_numHolyWater(10)
{}

GhostRacer::~GhostRacer() {}
// step 5
void GhostRacer::moveGhost() {
	static const double PI = 4 * atan(1.0);
	double max_shift_per_tick = 4.0;
	double delta_x = cos(getDirection() * 1.0 / 360 * 2 * PI) * max_shift_per_tick;
	double cur_x = getX();
	double cur_y = getY();
	moveTo(cur_x + delta_x, cur_y);
}
void GhostRacer::doSomething() {
	//check if the ghost racer dead
	if (getHP() <= 0) {
		setDead();
	}

	if (!isnotDead()) {
		return;
	}
	int ch;

	//check if the ghost racer is in a proper position
	if (getWorld()->validPos()) {
		moveGhost();
		return;
	}
	if (getWorld()->getKey(ch))
	{
		// user hit a key during this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (getDirection() < 114) {
				//Increase Ghost Racer’s current directional angle 8 degrees.
				setDirection(getDirection() + 8);
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() > 66) {
				//Decrease Ghost Racer’s current directional angle 8 degrees
				setDirection(getDirection() - 8);
			}
			break;
		case KEY_PRESS_SPACE:
			if (getNumSprays() >= 1) {
				//add new water projectile with proper position
				static const double PI = 4 * atan(1.0);
				double angle = getDirection() * 1.0 / 360 * 2 * PI;
				Actor* a = new Spray(getWorld(), getX() * cos(angle) +getX(),getY() * sin(angle) +getY() + SPRITE_HEIGHT, getDirection());
				getWorld()->addActor(a); // add holy water projectile
				getWorld()->playSound(SOUND_PLAYER_SPRAY); // play sound
				increaseSprays(-1); // decrease holy water amount
			}
			break;
		case KEY_PRESS_UP:
			if (getYspeed() < 5.0) {
				//speed up by 1 unit
				setYSpeed(1);
			}
			break;
		case KEY_PRESS_DOWN:
			if (getYspeed() > -1.0) {
				//slow down by 1 unit
				setYSpeed(-1);
			}
			break;
		default:
			break;
		}
	}
	moveGhost();
	return;
}

// How many holy water projectiles does the object have?
int GhostRacer::getNumSprays() const {
	return m_numHolyWater;
}

// Increase the number of holy water projectiles the object has.
void GhostRacer::increaseSprays(int amt) {
	m_numHolyWater = m_numHolyWater + amt;
}

// Spin as a result of hitting an oil slick.
void GhostRacer::spin() {
	int changeDir = getDirection() + randInt(5, 20);
	if (changeDir > 120) {
		setDirection(getDirection() - randInt(5, 20));
	}
	else {
		setDirection(getDirection() + randInt(5, 20));
	}
}

int GhostRacer::soundWhenDie() const {
	return SOUND_PLAYER_DIE;
}

// Border Line
BorderLine::BorderLine(StudentWorld* studentWorld, int imageID, int startX, int
	startY) :Actor(studentWorld, imageID, startX, startY, 0, 2.0, 2,-4)
{}

BorderLine::~BorderLine() {}
void BorderLine::doSomething() {
	//move the borderline accordingly
	if (!moveRelativeToGhostRacerVerticalSpeed(0)) {
		return;
	}
}

void BorderLine::damage() {}

//Pedestrain

//Pedestrain
//dir == 0
//hp == 2
Pedestrian::Pedestrian(StudentWorld* sw, int imageID, double x, double y, double size) :
Agent(sw,imageID,x,y,size,0,2)
{  
	//change the vertical speed for the pedestrain
	setYSpeed(-4);
}
Pedestrian:: ~Pedestrian() {}

int Pedestrian::soundWhenHurt() const {
	return SOUND_PED_HURT;
}

int Pedestrian::soundWhenDie() const {
	return SOUND_PED_DIE;
}


// Move the pedestrian.If the pedestrian doesn't go off screen and
// should pick a new movement plan, pick a new plan.
void Pedestrian::moveAndPossiblyPickPlan() {
	//just move
	if (!moveRelativeToGhostRacerVerticalSpeed(getHorizSpeed())) {
		return;
	}
	//check the move
	decreasePlan();
	if (getPlan() > 0) {
		return;
	}

	//re sethorizontal speed and check if needed to change plane
	int random = 0;
	while (random == 0) {
		random = randInt(-3, 3);
	}
	int horiz_speed = random;
	//change to a new plan
	setHorizSpeed(random);
	setPlan(randInt(4, 32));
	if (horiz_speed < 0) {
		setDirection(180);
	}
	else {
		setDirection(0);
	}
}

bool Pedestrian::beSprayedIfAppropriate() {
	return true;
}
//HumanPed
HumanPedestrian::HumanPedestrian(StudentWorld* sw, double x, double y) :Pedestrian(sw, IID_HUMAN_PED,x,y,2.0){}
HumanPedestrian::~HumanPedestrian() {}
void HumanPedestrian::doSomething() {
	if (!isnotDead()) {
		return;
	}
	//if overlap with ghost racer, then restart the game
	//player loses one life, communicate with student's world
	if (getWorld()->getOverlappingGhostRacer(this)) {
		//player loses one live
		getWorld()->getGhostRacer()->setHP(-100);
		setDead();
		return;
	}
	//move accordinly
	moveAndPossiblyPickPlan();
}

//if sprayed by the holy water 
void HumanPedestrian::damage() {
	takeDamageAndPossiblyDie(0);
}



//if hit by the water projectile
bool HumanPedestrian::takeDamageAndPossiblyDie(int hp) {
	//set horizontal speed to opposite
	int xspeed = getHorizSpeed() * (-1);
	setHorizSpeed(xspeed);
	//change the diretion
	if (getDirection() == 0) {
		setDirection(180);
	}
	else {
		setDirection(0);
	}
	getWorld()->playSound(soundWhenHurt());
	return true;
}

//Zombie pedestrian
ZombiePedestrian::ZombiePedestrian(StudentWorld* sw, double x, double y) :Pedestrian(sw, IID_ZOMBIE_PED,x,y,3.0), m_tick(0){}

ZombiePedestrian::~ZombiePedestrian() {}
void ZombiePedestrian::doSomething() {
	if (!isnotDead()) {
		return;
	}
	
	//if overlap with ghost racer
	if (getWorld()->getOverlappingGhostRacer(this)) {
		getWorld()->getGhostRacer()->takeDamageAndPossiblyDie(5);//The Ghost Racer must receive 5 points of damage
		takeDamageAndPossiblyDie(2);//the zombie ped dies
		getWorld()->playSound(soundWhenDie());
		getWorld()->increaseScore(150);//Ensure the player receives 150 points
		return;
	}

	//if within 30 pixes around ghost racer AND the zombie pedestrian is in front of the Ghost
	//Racer on the road
	if ((abs(getX()-getWorld()->getGhostRacer()->getX())<=30)&&(getY()> getWorld()->getGhostRacer()->getY())) {
		setDirection(270);
		//set th horizontal speed accordingly
		if (getX() - getWorld()->getGhostRacer()->getX() < 0) {
			setHorizSpeed(1);
		}
		else if (getX() - getWorld()->getGhostRacer()->getX() > 0) {
			setHorizSpeed(-1);
		}
		else {
			setHorizSpeed(0);
		}
		m_tick--;
		if (m_tick <= 0) {
			getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
			m_tick = 20;
		}
	}
	//grunts every 20 ticks, it is kinda annoying XD
	moveAndPossiblyPickPlan();
}


//when damaged by the holy water
void ZombiePedestrian::damage() {
	//reduced the hp
	setHP(-1);
	//if died under holy water
	if (getHP() <= 0) {
		//a 1 in 5 chance that the zombie ped will add a new healing
			//goodie at its current position
		getWorld()->playSound(soundWhenDie());
		int i = randInt(1, 5);
		if (i == 1) {
			Actor* a = new HealingGoodie(getWorld(), getX(), getY());
			getWorld()->addActor(a);
		}
		//increase score
		getWorld()->increaseScore(150);
		setDead();
	}
	getWorld()->playSound(soundWhenHurt());
}
//zombie cab

ZombieCab::ZombieCab(StudentWorld* sw, double x, double y) :
	Agent(sw, IID_ZOMBIE_CAB, x, y, 4.0, 90, 3), m_damage(false)
{}

ZombieCab::~ZombieCab() {}
void ZombieCab::doSomething() {
	if (!isnotDead()) {
		return;
	}
	//if overlap with ghost racer
	if (getWorld()->getOverlappingGhostRacer(this)) {
		//If the zombie cab has already damaged Ghost Racer, it must skip this step
		if (m_damage == false) {
			//damage the ghost racer and itself
			getWorld()->playSound(SOUND_VEHICLE_CRASH);
			getWorld()->getGhostRacer()->takeDamageAndPossiblyDie(20);
		    getWorld()->increaseScore(200);
			//change to a proper angle and horizontal speed to drives out of the screen and die
			if (getX() - getWorld()->getGhostRacer()->getX() < 0 || getX() == getWorld()->getGhostRacer()->getX()) {
				setHorizSpeed(-5);
				setDirection(120 + randInt(0, 19));
			}
			else {
				setHorizSpeed(5);
				setDirection(60 - randInt(0, 19));
			}
			m_damage = true;
		}
	}

	moveRelativeToGhostRacerVerticalSpeed(getHorizSpeed());

	//check if there is a collision worthy actor in front of the zombie cab and then decrease speed
	if (getYspeed() > getWorld()->getGhostRacer()->getYspeed()) {
		int yc = getWorld()->aboveZombieCab(this, true);
		if (yc != -1) {
			if (yc - getY() < 96) {
					setYSpeed(-0.5);
					return;
				}
		}
	}
	//check if there is a collision worthy actor behind the zombie cab and then decrease speed
	if (getYspeed() < getWorld()->getGhostRacer()->getYspeed()|| getYspeed() == getWorld()->getGhostRacer()->getYspeed()) {
		int yc = getWorld()->aboveZombieCab(this,false);
		if (yc != -1) {
			if (getY() - yc < 96) {
				setYSpeed(0.5);
				return;
			}
		}
	}
	decreasePlan();
	if (getPlan() > 0) {
		return;
	}
	setPlan(randInt(4, 32));
	setYSpeed(randInt(-2, 2));
}

void ZombieCab::damage() {
	//decrease hp
	setHP(-1);
	//if zombie cab died under water
	if (getHP() <= 0) {
		setDead();
		getWorld()->playSound(soundWhenDie());
		getWorld()->increaseScore(200);
		//a 1 in 5 chance that the zombie cab will add a new oil
		//slick at its current position.
		int i = randInt(1, 5);
		if (i ==1) {
			Actor* a = new OilSlick(getWorld(), getX(), getY());
			getWorld()->addActor(a);
		}
	}
	getWorld()->playSound(soundWhenHurt());
}

//maybe add new lines after finish water projectile part
bool ZombieCab::beSprayedIfAppropriate() {
	return true;
}

//spray
Spray::Spray(StudentWorld* sw, double x, double y, int dir) :Actor(sw, IID_HOLY_WATER_PROJECTILE,x,y,dir,1.0,1,0), m_travelDis(0)
{}

Spray::~Spray() {}
void Spray::doSomething() {
	if (!isnotDead()) {
		return;
	}
	//check if water projectile overlapped with each other
	bool hitOneActor = getWorld()->affectedwithProjectile(this);
	if (hitOneActor)
		return;
	//move the water projectile and set to dead if in invalid position
	moveForward(SPRITE_HEIGHT);
	m_travelDis = m_travelDis + SPRITE_HEIGHT;
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
		setDead();
		return;
	}
	if (m_travelDis >= 160) {
		setDead();
		return;
	}
}

void Spray::damage() {}


//goodie
GhostRacerActivatedObject::GhostRacerActivatedObject(StudentWorld* sw, int imageID, double x, double y, double size, int dir):
	Actor(sw,imageID,x,y,dir,size,2,-4)
{}
GhostRacerActivatedObject::~GhostRacerActivatedObject() {}

bool GhostRacerActivatedObject::beSprayedIfAppropriate() {
	return true;
}

int GhostRacerActivatedObject::getSound() const {
	return SOUND_GOT_GOODIE;
}

//do general activity for all the goodie
void GhostRacerActivatedObject::doGeneralActivity(){
	getWorld()->playSound(getSound());//play sound
	getWorld()->increaseScore(getScoreIncrease());//increase proper score
	if (selfDestructs()) {//destruct itself
		setDead();
	}
}

//if overlapped with holy water, delete this goodie
void GhostRacerActivatedObject::damage() {
	setDead();
}

void GhostRacerActivatedObject::doSomething() {
	//move the goodie
	if (!moveRelativeToGhostRacerVerticalSpeed(0)) {
		return;
	}

	//if overlap with the ghost racer, then call the do activity()
	if (getWorld()->getOverlappingGhostRacer(this)) {
		doActivity(getWorld()->getGhostRacer());
	}
}

bool GhostRacerActivatedObject::selfDestructs() const {
	return true;
}
//oil slicks
OilSlick::OilSlick(StudentWorld* sw, double x, double y):GhostRacerActivatedObject(sw, IID_OIL_SLICK,x,y,randInt(2,5),0){}

void OilSlick::doActivity(GhostRacer* gr) {
	gr->spin();
	doGeneralActivity();
}
OilSlick::~OilSlick() {}
int OilSlick::getScoreIncrease() const {
	//does oil slick increase score?
	return 0;
}

int OilSlick::getSound() const {
	return SOUND_OIL_SLICK;
}

//oil slick cannot be self destruct
bool OilSlick::selfDestructs() const {
	return false;
}

bool OilSlick::beSprayedIfAppropriate() {
	return false;
}
HealingGoodie::HealingGoodie(StudentWorld* sw, double x, double y) :GhostRacerActivatedObject(sw, IID_HEAL_GOODIE,x,y,1.0,0){}

HealingGoodie::~HealingGoodie() {}

void HealingGoodie::doActivity(GhostRacer* gr) {
	//increase hp by 10 pts
		gr->setHP(10);
		doGeneralActivity();
}

int HealingGoodie::getScoreIncrease() const {
	return 250;
}


HolyWaterGoodie::HolyWaterGoodie(StudentWorld* sw, double x, double y) :GhostRacerActivatedObject(sw, IID_HOLY_WATER_GOODIE,x,y,2.0,90){}

HolyWaterGoodie::~HolyWaterGoodie() {}
void HolyWaterGoodie::doActivity(GhostRacer* gr) {
	//increase the number of holy water by 10
	gr->increaseSprays(10);
	doGeneralActivity();
}

int HolyWaterGoodie::getScoreIncrease() const {
	return 50;
}


SoulGoodie::SoulGoodie(StudentWorld* sw, double x, double y) :GhostRacerActivatedObject(sw, IID_SOUL_GOODIE,x,y,4.0,0){
}

void SoulGoodie::doSomething() {
	GhostRacerActivatedObject::doSomething();
	//rotate the object 10 degree clockwise
	setDirection(getDirection() - 10);
}
SoulGoodie::~SoulGoodie() {}

void SoulGoodie::doActivity(GhostRacer* gr) {
//Increase the number of lost souls
	doGeneralActivity();
	getWorld()->incSouls();
}

int SoulGoodie::getScoreIncrease() const {
	return 100;
}

// Return the sound to be played when the object is activated.
int SoulGoodie::getSound() const {
	return SOUND_GOT_SOUL;
}
