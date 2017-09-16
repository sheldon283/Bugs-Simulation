#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include "Compiler.h"
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	//Constructor
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir), m_ticks(0)
	{}
	
	//Destructor
	~StudentWorld() {
		cleanUp();
	}
	
	//Accessors
	Actor* getActorAtProposedLocation(int x, int y);
	
	int getCurrentTicks() const { return m_ticks; }

	//Mutators
	bool isNextPebble(int x, int y);

	bool isPebble(int x, int y);

	bool isThereFood(int x, int y);

	bool isThereAnthill(int x, int y, int colony);
	
	bool isTherePheromone(int x, int y);

	int loadTheField(Field &f);

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	list<Actor*> getActorList(int x, int y) {
		return myActors[x][y];
	}

	void setActorList(int x, int y, Actor* a) {
		myActors[x][y].push_back(a);
	}

	
	void takeHealthFromInsect(int a, int x, int y, int damage, Actor* b);

	string formatThingsNicely(int ticks, int first, int second, int third, int fourth, int winning);

	stack<pair<int, int>> getCoordStack() const { return deadCoordinates; }

	stack<Actor*> getActorStack() const { return deadActors; }

	stack<Actor*> pushActorStack(Actor* a) {
		deadActors.push(a);
		return deadActors;
	}

	Actor* checkSpace(int x, int y, string search);
	
	stack<pair<int, int>> pushCoordStack(int x, int y) {
		pair<int, int> coordinate = { x, y };
		deadCoordinates.push(coordinate);
		return deadCoordinates;
	}

	void createNewObject(int x, int y, string className, int health, int colony);

	void stunObjectsOnSquare(int x, int y);

	void poisonObjectsOnSquare(int x, int y);

	void removeDeadSimulationObjects();

	void updateDisplayText();

	void setAntsProduced(int colony) {
		antsProduced[colony]++;
	}
private:
	int m_ticks;
	list<Actor*> myActors[VIEW_WIDTH][VIEW_HEIGHT];
	stack<pair<int,int>> deadCoordinates;
	stack<Actor*> deadActors;
	int antsProduced[4] = { 0, 0, 0, 0 };
	vector<string> antNames;
	vector<string> fileNames;
};

#endif // STUDENTWORLD_H_
