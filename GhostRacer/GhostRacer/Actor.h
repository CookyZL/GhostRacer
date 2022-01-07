#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
// Students: Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor :public GraphObject {
public:
	//constructor
	Actor(StudentWorld* studentWorld, int imageID, double startX, double startY, int dir,
		double size, unsigned int depth, int Yspeed);
	virtual ~Actor();


	// Action to perform for each tick.
	virtual void doSomething() = 0;

	// Is this actor dead?
	bool isnotDead() const;

	// Does this object affect zombie cab placement and speed?
	virtual bool collisionAvoidanceActor() const;

	// Set this actor's vertical speed.
	void setYSpeed(double num);

	// Mark this actor as dead.
	void setDead();

	// Get this actor's vertical speed.
	double getYspeed() const;

	//action performed when damaged by holy water projectile
	virtual void damage() = 0;

	// If this actor can be affected by holy water projectiles, then return true; otherwise, return false.
	virtual bool beSprayedIfAppropriate();

protected:
	// Adjust the x coordinate by dx to move to a position with a y coordinate
	  // determined by this actor's vertical speed relative to GhostRacser's
	  // vertical speed.  Return true if the new position is within the view;
	  // otherwise, return false, with the actor dead.
	bool moveRelativeToGhostRacerVerticalSpeed(double dx);

	//return a pointer to StudentWorld
	StudentWorld* getWorld();

private:
	StudentWorld* m_studentWorld;
	bool m_alive;
	double m_Yspeed;
};

//Border Line
class BorderLine :public Actor {
public:
	BorderLine(StudentWorld* studentWorld, int imageID, int startX, int startY);
	virtual ~BorderLine();
	virtual void doSomething();
	virtual void damage();

private:
};

//Actor with HIT pts
class Agent :public Actor {
public:
	Agent(StudentWorld* sw, int imageID, double x, double y, double size, int dir, int hp);

	//destructor
	virtual ~Agent();
	virtual bool collisionAvoidanceActor() const;

	// Get hit points.
	int getHP() const;

	//decrease one hit
	virtual void damage();

	// Increase hit points by hp.
	void setHP(int hp);

	// Do what the spec says happens when hp units of damage is inflicted.
	// Return true if this agent dies as a result, otherwise false.
	virtual bool takeDamageAndPossiblyDie(int hp);

protected:
	//set plan to a new number
	void setPlan(int s);

	//decrement plan distance by 1
	void decreasePlan();

	//get the plan distance
	int getPlan() const;

	// Set the pedestrian's and zombie cab horizontal speed
	void setHorizSpeed(int s);

	// Get the pedestrian's and zombie cab horizontal speed
	int getHorizSpeed() const;

	// What sound should play when this agent is damaged and dies?
	virtual int soundWhenDie();

	// What sound should play when this agent is damaged but does not die?
	virtual int soundWhenHurt();

private:
	int m_hit;
	int x_speed;
	int m_movementPlanDis;
};


//ghost racer
class GhostRacer :public Agent {
public:
	GhostRacer(StudentWorld* studentWorld);
	virtual void doSomething();
	virtual ~GhostRacer();
	// How many holy water projectiles does the object have?
	int getNumSprays() const;

	// Spin as a result of hitting an oil slick.
	void spin();

	// Increase the number of holy water projectiles the object has.
	void increaseSprays(int amt);

protected:
	virtual int soundWhenDie() const;

private:
	void moveGhost();
	int m_numHolyWater;
};

class Pedestrian : public Agent
{
public:
	Pedestrian(StudentWorld* sw, int imageID, double x, double y, double size);
	virtual ~Pedestrian();
	virtual bool beSprayedIfAppropriate();

protected:
	virtual int soundWhenHurt() const;
	virtual int soundWhenDie() const;
	// Move the pedestrian.  If the pedestrian doesn't go off screen and
	// should pick a new movement plan, pick a new plan.
	void moveAndPossiblyPickPlan();

private:
};

class HumanPedestrian : public Pedestrian
{
public:
	HumanPedestrian(StudentWorld* sw, double x, double y);
	virtual ~HumanPedestrian();
	virtual void doSomething();
	virtual bool takeDamageAndPossiblyDie(int hp);
	virtual void damage();
private:

};

class ZombiePedestrian : public Pedestrian
{
public:
	ZombiePedestrian(StudentWorld* sw, double x, double y);
	virtual ~ZombiePedestrian();
	virtual void doSomething();
	virtual void damage();
private:
	int m_tick;
};

class ZombieCab : public Agent
{
public:
	ZombieCab(StudentWorld* sw, double x, double y);
	~ZombieCab();
	virtual void doSomething();
	virtual bool beSprayedIfAppropriate();
	virtual void damage();
private:
	bool m_damage;
};

class Spray : public Actor
{
public:
	Spray(StudentWorld* sw, double x, double y, int dir);
	virtual ~Spray();
	virtual void doSomething();
	virtual void damage();
private:
	int m_travelDis;
};

class GhostRacerActivatedObject : public Actor
{
public:

	GhostRacerActivatedObject(StudentWorld* sw, int imageID, double x, double y, double size, int dir);
	virtual bool beSprayedIfAppropriate();
	virtual ~GhostRacerActivatedObject();

	//destrory the goodie
	virtual void damage();

	//create a common dosomething function for some goodie
	virtual void doSomething();

protected:
	//create a common function for general activity for each goodie
	void doGeneralActivity();

	// Return the sound to be played when the object is activated.
	virtual int getSound() const;

	// Return whether the object dies after activation.
	virtual bool selfDestructs() const;

	// Do the object's special activity (increase health, spin Ghostracer, etc.)
	virtual void doActivity(GhostRacer* gr) = 0;

	// Return the object's increase to the score when activated.
	virtual int getScoreIncrease() const = 0;

};

class OilSlick:public GhostRacerActivatedObject
{
public:
	OilSlick(StudentWorld* sw, double x, double y);
	virtual ~OilSlick();
	virtual bool beSprayedIfAppropriate();
protected:
	virtual void doActivity(GhostRacer* gr);
	virtual int getScoreIncrease() const;
	virtual int getSound() const;
	virtual bool selfDestructs() const;
	//remember that all objects except ghost racer cannot be spunned by oil slick
};

class HealingGoodie : public GhostRacerActivatedObject
{
public:
	HealingGoodie(StudentWorld* sw, double x, double y);
	virtual ~HealingGoodie();
protected:
	virtual void doActivity(GhostRacer* gr);
	virtual int getScoreIncrease() const;
};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
	HolyWaterGoodie(StudentWorld* sw, double x, double y);
	virtual ~HolyWaterGoodie();
protected:
	virtual void doActivity(GhostRacer* gr);
	virtual int getScoreIncrease() const;
};

class SoulGoodie : public GhostRacerActivatedObject
{
public:
	SoulGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual ~SoulGoodie();

protected:
	virtual void doActivity(GhostRacer* gr);
	virtual int getScoreIncrease() const;
	virtual int getSound() const;
};

#endif // ACTOR_H_