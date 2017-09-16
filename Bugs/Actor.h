#ifndef ACTOR_H_
#define ACTOR_H_

#include <iostream>
#include <string>
#include <cmath>
#include <math.h>
#include <random>
#include "GraphObject.h"
#include "Compiler.h"

class StudentWorld;

using namespace std;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp'

class Actor : public GraphObject {
public:
	//Constructor
	Actor(StudentWorld* s, int imageID, int x, int y, Direction direction, double size, int depth) : GraphObject(imageID, x, y, direction, depth, size), m_world(s), ID(imageID) {
		m_stunnable = false;
		m_poisonable = false;
		m_isEdible = false;
		m_bitable = false;
		m_block = false;
		m_moveBlock = false;
	}

	//Destructor
	virtual ~Actor() { 
		//delete m_world;
		return;
	}

	//Accessors
	bool doIBlock() const { return m_block; }
	StudentWorld* getWorld() const { return m_world; }
	bool getStunability() const { return m_stunnable; }
	bool getPoisonability() const { return m_poisonable; }
	bool getEdibility() const { return m_isEdible; }
	bool getBitable() const { return m_bitable; }
	bool getIsGrasshopper() const { return m_isGrasshopper; }
	virtual bool isPheromone() const { return false; }
	bool getMoveBlock() const { return m_moveBlock; }
	virtual bool isDead() const { return false; }
	virtual bool getJustStunned() const { return true; }
	virtual bool getJustPoisoned() const { return true; }
	virtual bool getJustBit() const { return true; }
	virtual int getHealth() const { return 0; cout << "getHealth should not be called" << endl; }
	virtual int getID() const { return ID; }
	virtual bool isEnemy(int colony) const { return false; }
	virtual int getColony() const { return 4; }

	//Mutators
	void setStunability(bool value) { m_stunnable = value; }
	void setPoisonability(bool value) { m_poisonable = value; }
	void setEdibility(bool value) { m_isEdible = value; }
	void setBitable(bool value) { m_bitable = value; }
	void setIsGrasshopper(bool value) { m_isGrasshopper = value; }
	void setMoveBlock(bool value) { m_moveBlock = value; }
	virtual void setJustStunned(bool value) { return; cout << "This setJustStunned should not be called" << endl; }
	virtual void setJustPoisoned(bool value) { return; cout << "This setJustPoisoned should not be called" << endl; }
	virtual void setJustBit(bool value) { return; cout << "This setJustBit should not be called" << endl; }
	virtual void setSleep(bool value) { return; cout << "This setSleep should not be called" << endl; }
	virtual void setSleepTicks(int amt) { return; cout << "This setSleepTicks should not be called" << endl; }
	virtual void addSleepTicks(int amt) { return; cout << "This addSleepTicks should not be called" << endl; }

	virtual void gainHealth(int amount) { return; cout << "This gainHealth should not be called" << endl; }
	virtual void loseHealth(int amount) { return; cout << "This loseHealth should not be called" << endl; }

	void changeBlockStatus() { m_block = true; }
	GraphObject::Direction generateRandomDirection();

	virtual void doSomething() { return; }

private:
	bool m_block;
	StudentWorld* m_world;
	bool m_stunnable;
	bool m_poisonable;
	bool m_isEdible;
	bool m_isGrasshopper;
	bool m_bitable;
	bool m_moveBlock;
	int ID;
};

class HitpointObject : public Actor {
public:
	//Constructor
	HitpointObject(StudentWorld* s, int imageID, int x, int y, int hitpoints, Direction direction, double size, int depth) : Actor(s, imageID, x, y, direction, size, depth), m_hitPoints(hitpoints), m_continueMoving(false) {
		m_instructions = { direction, 0 };
		setStunability(false);
		setPoisonability(false);
		setIsGrasshopper(false);
		m_eatBool = false;
	}

	//Destructors
	virtual ~HitpointObject() { return; }

	//Accessors
	virtual bool isDead() const { return m_hitPoints <= 0; }	//Checks if object is dead
	virtual int getHealth() const { return m_hitPoints; }	//returns the current health of object
	bool continueMoving() const { return m_continueMoving; }	//
	pair<Direction, int> getInstructions() const { return m_instructions; }	//Returns the instructions
	bool getEatStatus() const { return m_eatBool; }	//returns whether or not the insect has eaten

	//Mutators
	virtual void setDead() { m_dead = true; }	//Changes dead/alive status
	virtual void gainHealth(int amount) { m_hitPoints += amount; }	//Add certain amount of health to object
	virtual void loseHealth(int amount) { m_hitPoints -= amount; }	//Subtract certain amount of health from object
	void loseHealthOnTick() { m_hitPoints--; }	//Decreases health on every tick by one
	int  setHealth(int amt) {	//Sets health of objects to amount
		m_hitPoints = amt;
		return m_hitPoints;
	}

	pair<Direction, int> setInstructions(Direction d, int distance) {	//Returns the instructions of object
		return m_instructions = { d, distance };
	}

	void setMovingToFalse() { m_continueMoving = false; }
	void setMovingToTrue() { m_continueMoving = true; }
	void moveDistanceInDirection(int x, int y, int antOrNot);	//Move object in direction by following directions
	void makeDead(string name = "");	//Kill the object and add generate food
	void makeDead(Actor* a);

	void setEatStatus(bool value) { m_eatBool = value; }	//Change the status of whether the insect has eaten

private:
	bool m_dead;
	int m_hitPoints;
	bool m_continueMoving;
	pair<Direction, int> m_instructions;
	bool m_eatBool;
};

class Insect : public HitpointObject {
public:
	//Constructor
	Insect(StudentWorld* s, int ID, int x, int y, int hitpoints, Direction d, double size, int depth) : HitpointObject(s, ID, x, y, hitpoints, d, size, depth), m_sleep(false) {
		m_poisonStatus = false;
		setBitable(true);
		setJustBit(false);
		setJustPoisoned(false);
		setJustStunned(false);
	}

	//Destructor
	virtual ~Insect() { return; }

	//Accessors
	
	bool getPoisonStatus() const { return m_poisonStatus; }	//returns whether the insect is poisoned
	virtual bool getJustStunned() const { return m_justStunned; }	//Returns whether the insect has just been stunned
	virtual bool getJustPoisoned() const { return m_justPoisoned; }
	virtual bool getJustBit() const { return m_justBit; }
	int getSleepTicks() const { return m_sleepTicks; }	//returns how many ticks the insect has to sleep
	bool isSleep() const { return m_sleep; }	//returns whether the object is sleeping/stunned

	//Mutators	
	virtual void setJustStunned(bool value) { m_justStunned = value; }	//changes justStunned value
	virtual void setJustPoisoned(bool value) { m_justPoisoned = value; }
	virtual void setJustBit(bool value) { m_justBit = value; }
	virtual void move() { return; }	//Move the object
	void eat();	//Eat food on square
	void bite(int damage, int colony);	//Bite other insects on square
	void setPoisonStatus(bool value) { m_poisonStatus = value; }	//Change the poison status of the insect
	virtual void getBit() { return; }	//Make the insect take damage from being bit
	virtual void doSomething() { return; }	

	virtual void setSleepTicks(int amt) { m_sleepTicks = amt; }	//Set how many ticks for insect to sleep to amount
	void decreaseSleep() { m_sleepTicks--; }	//Decrease the sleepTicks by one
	void setSleep(bool value) { m_sleep = value; }	//Change the bool for sleep
	virtual void addSleepTicks(int amt) { m_sleepTicks += amt; }	//Add certain amount of sleepTicks to current amount
	void checkIfSleep();	//Checks if object is sleeping and decreases sleep counter

private:
	bool m_eatBool;
	bool m_poisonStatus;
	bool m_justStunned;
	bool m_justPoisoned;
	bool m_sleep;
	int m_sleepTicks;
	bool m_justBit;
};

class Ant : public Insect {
public:
	//Constructor
	Ant(StudentWorld* s, int imageID, int x, int y, int num, Compiler* c) : Insect(s, imageID, x, y, 1500, generateRandomDirection(), 0.25, 1), m_colony(num), m_compiler(c), m_foodCarry(0), ic(0), m_random(0) {
		setPoisonability(true);
	}

	//setPoisonability(true);

	//Destructor
	virtual ~Ant() { 
		delete m_compiler;
	}

	//Accessors
	int getFoodCarry() const { return m_foodCarry; }
	Compiler* getCompiler() const { return m_compiler; }
	int getRandom() const { return m_random; }
	virtual int getColony() const { return m_colony; }
	virtual bool isEnemy(int colony) const { return m_colony != colony; }

	//Mutators
	void compileFunction() { return; }
	void setRandom(int num) { m_random = num; }
	void addFoodCarry(int amt) { m_foodCarry += amt; }
	virtual void doSomething();// { return; }

private:
	Compiler* m_compiler;
	int m_random;
	int m_foodCarry;
	int ic;
	int m_colony;
};

class Grasshopper : public Insect {
public:
	//Constructor
	Grasshopper(StudentWorld* s, int imageID, int x, int y, int hitpoints) : Insect(s, imageID, x, y, hitpoints, generateRandomDirection(), 0.25, 1) {
		setInstructions(getDirection(), randInt(2,10));
	}

	//Destructor
	virtual ~Grasshopper() { return; }

	//Accessors
	virtual bool isEnemy(int colony) const { return true; }

	//Mutators

	virtual void doSomething() { return; }


private:

};

class Adult : public Grasshopper {
public:
	//Constructor
	Adult(StudentWorld* s, int x, int y) : Grasshopper(s, IID_ADULT_GRASSHOPPER, x, y, 1600) {
		setStunability(false);
		setIsGrasshopper(true);
	}

	//Destructor
	virtual ~Adult() { return; }

	//Accessors

	//Mutators
	virtual void doSomething();// { return; }
	void generateRadius(int& finalX, int& finalY);
	void jump();
};

class Baby : public Grasshopper {
public:
	//Constructor
	Baby(StudentWorld* s, int x, int y) : Grasshopper(s, IID_BABY_GRASSHOPPER, x, y, 500) {
		setPoisonability(true);
		setStunability(true);
	}
	//HOW TO DECLARE THE HITPOINTS OF BABY GRASSHOPPER USING GRASSHOPPER CLASS

	//Destructor
	virtual ~Baby() { return; }

	//Accessors


	//Mutators
	virtual void doSomething();

private:

};

class Anthill : public HitpointObject {
public:
	//Constructor
	Anthill(StudentWorld* s, int num, int x, int y, Compiler compiler) : HitpointObject(s, IID_ANT_HILL, x, y, 8999, right, 0.25, 2), m_colonyNumber(num), m_compiler(compiler) {}

	//Destructor
	virtual ~Anthill() { 
	//	delete m_compiler;
		return;
	}

	//Accessors
	virtual int getColony() const { return m_colonyNumber; }
	Compiler getCompiler() const { return m_compiler; }

	//Mutators
	virtual void doSomething();

private:
	int m_colonyNumber;
	Compiler m_compiler;

};

class Food : public HitpointObject {
public:
	//Constructor
	Food(StudentWorld* s, int x, int y, int hitpoints) : HitpointObject(s, IID_FOOD, x, y, hitpoints, right, 0.25, 2) {
		setEdibility(true);
	}

	//Destructor
	virtual ~Food() { return; }

	//Accessors
	

	//Mutators
	virtual void doSomething() { return; }

private:

};

class Pheromones : public HitpointObject {
public:
	//Constructor
	Pheromones(StudentWorld* s, int imageID, int x, int y, int hitpoints, int colony) : HitpointObject(s, imageID, x, y, 256, right, 0.25, 2), m_colony(colony) {}

	//Destructor
	virtual ~Pheromones() { return; }

	//Accessors
	virtual bool isEnemy(int colony) const { return m_colony != colony; }
	virtual int getColony() const { return m_colony; }
	virtual bool isPheromone() const { return true; }

	//Mutators
	virtual void doSomething();

private:
	int m_colony;
};

class Poison : public Actor {
public:
	//Constructor
	Poison(StudentWorld* s, int x, int y) : Actor(s, IID_POISON, x, y, right, 0.25, 2) {}

	//Destructor
	virtual ~Poison() { return; }

	//Accessors


	//Mutators	
	void poison();
	virtual void doSomething();// { return; }

private:
};

class Water : public Actor {
public:
	//Constructor
	Water(StudentWorld* s, int x, int y) : Actor(s, IID_WATER_POOL, x, y, right, 0.25, 2) {}

	//Destructor
	virtual ~Water() { return; }

	//Accessors


	//Mutators
	virtual void doSomething();

private:
};

class Pebble : public Actor {
public:
	//Constructor
	Pebble(StudentWorld* s, int x, int y) : Actor(s,IID_ROCK, x, y, right, 0.25, 1) {
		changeBlockStatus();
	}

	//Destructor
	virtual ~Pebble() { return; }

	//Accessors

	//Mutators
	virtual void doSomething() { return; }

};

#endif // ACTOR_H_
