#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include <string>
// Students: Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
#include <vector>
#include "Actor.h"
#include <iostream>
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	//return the ptr to ghost racer
	GhostRacer* getGhostRacer() const;

	//check if ghost racer is in valid position
	bool validPos();
	// If actor a overlaps this world's GhostRacer, return a pointer to the
	  // GhostRacer; otherwise, return nullptr
	GhostRacer* getOverlappingGhostRacer(Actor* a) const;

	//a function to check if a "collision-avoidance worthy" actor
	//in the zombie cab's lane that is in front of or behind that zombie cab
	int aboveZombieCab(const Actor* a, bool isFront);


	//check if two actors overlap with eachother
	bool overlaps(const Actor* a1, const Actor* a2) const;

	//check if any object is overlap with the holy water
	//do activity in that object
	bool affectedwithProjectile(Actor* a1);
	// Add an actor to the world.
	void addActor(Actor* a);
	//increase the number of souls, when the ghost racer overlap the lost soul
	void incSouls();
protected:
	//get number of souls
	int getSouls() const;


	// check whether a specific line is a proper line to insert a zombie cab
	bool addNewZombieCab(int num, int& vspeed, int& ycordinate, int& xc);

private:
	GhostRacer* m_GhostRacer;
	std::vector<Actor*> m_actors;
	int m_Ycoordinate;
	int score_remaining;
	int m_souls2save;
};
#endif // STUDENTWORLD_H_D