#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
int StudentWorld::init() {
	Field f;
	loadTheField(f);
//	Compiler a0, a1, a2, a3;
//	Compiler compilerForEntrant0, compilerForEntrant1,
//		compilerForEntrant2, compilerForEntrant3;
	Anthill* ah0, *ah1, *ah2, *ah3;
	// get the names of all of the ant program source files
	// we provide the getFilenamesOfAntPrograms() function for
	// you in our GameWorld class.
	std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
	//	compilerForEntrant0 = new Compiler;
	//	compilerForEntrant1 = new Compiler;
	//	compilerForEntrant2 = new Compiler;
	//	compilerForEntrant3 = new Compiler;
	std::string error;

	int h0x = 0, h0y = 0, h1x = 0, h1y = 0, h2x = 0, h2y = 0, h3x = 0, h3y = 0;
	for (int x = 0; x < VIEW_WIDTH; x++) {
		for (int y = 0; y < VIEW_HEIGHT; y++) {
			Field::FieldItem item = f.getContentsOf(x, y); // note it’s x,y and not y,x!!!
			if (item == Field::FieldItem::rock)
				myActors[x][y].push_back(new Pebble(this, x, y));	//Make Pebble
			else if (item == Field::FieldItem::grasshopper)
				myActors[x][y].push_back(new Baby(this, x, y));	//Make baby grasshopper
			else if (item == Field::FieldItem::water)
				myActors[x][y].push_back(new Water(this, x, y));	//Make water
			else if (item == Field::FieldItem::poison)
				myActors[x][y].push_back(new Poison(this, x, y));	//Make poison
			else if (item == Field::FieldItem::food)
				myActors[x][y].push_back(new Food(this, x, y, 6000));	//Make food
			else if (item == Field::FieldItem::anthill0) {
				h0x = x;
				h0y = y;
				//				ah0 = new Anthill(this, 0, x, y, compilerForEntrant0);
				//				antNames.push_back(compilerForEntrant0->getColonyName());
				//				myActors[x][y].push_back(ah0);	//Make Anthill0
			}
			else if (item == Field::FieldItem::anthill1) {
				h1x = x;
				h1y = y;
				//				ah1 = new Anthill(this, 1, x, y, compilerForEntrant1);
				//				antNames.push_back(compilerForEntrant1->getColonyName());
				//				myActors[x][y].push_back(ah1);	//Make Anthill0
			}
			else if (item == Field::FieldItem::anthill2) {
				h2x = x;
				h2y = y;
				//				ah2 = new Anthill(this, 2, x, y, compilerForEntrant2);
				//				antNames.push_back(compilerForEntrant2->getColonyName());
				//				myActors[x][y].push_back(ah2);	//Make Anthill0
			}
			else if (item == Field::FieldItem::anthill3) {
				h3x = x;
				h3y = y;
				//				ah3 = new Anthill(this, 3, x, y, compilerForEntrant3);
				//				antNames.push_back(compilerForEntrant3->getColonyName());
				//				myActors[x][y].push_back(ah3);	//Make Anthill0
			}
		}
	}

	if (fileNames.size() >= 1) {
		Compiler compilerForEntrant0;
		if (!compilerForEntrant0.compile(fileNames[0], error))
		{
			// entrant 0’s source code had a syntax error!
			// send this error to our framework to warn the user.
			// do it JUST like this!
			setError(fileNames[0] + " " + error);
			// return an error to tell our simulation framework
			// that something went wrong, and it’ll inform the user
			return GWSTATUS_LEVEL_ERROR;
		}
		ah0 = new Anthill(this, 0, h0x, h0y, compilerForEntrant0);
		antNames.push_back(compilerForEntrant0.getColonyName());
		myActors[h0x][h0y].push_back(ah0);	//Make Anthill0
	}
	if (fileNames.size() >= 2) {
		Compiler compilerForEntrant1;
		if (!compilerForEntrant1.compile(fileNames[1], error))
		{
			setError(fileNames[1] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
		ah1 = new Anthill(this, 1, h1x, h1y, compilerForEntrant1);
		antNames.push_back(compilerForEntrant1.getColonyName());
		myActors[h0x][h0y].push_back(ah1);	//Make Anthill1
	}
	if (fileNames.size() >= 3) {
		Compiler compilerForEntrant2;
		if (!compilerForEntrant2.compile(fileNames[2], error))
		{
			setError(fileNames[2] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
		ah2 = new Anthill(this, 2, h2x, h2y, compilerForEntrant2);
		antNames.push_back(compilerForEntrant2.getColonyName());
		myActors[h0x][h0y].push_back(ah2);	//Make Anthill2
	}
	if (fileNames.size() >= 4) {
		Compiler compilerForEntrant3;
		if (!compilerForEntrant3.compile(fileNames[3], error))
		{
			setError(fileNames[3] + " " + error);
			return GWSTATUS_LEVEL_ERROR;
		}
		ah3 = new Anthill(this, 3, h3x, h3y, compilerForEntrant3);
		antNames.push_back(compilerForEntrant3.getColonyName());
		myActors[h3x][h3y].push_back(ah3);	//Make Anthill3
	}

	m_ticks = 0;

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
	if (m_ticks < 2000) {	//While game is still running

		vector<pair<int, int>> old_pos;
		vector<pair<int, int>> new_pos;
		vector<Actor*> object;

		m_ticks++;

		//Let each actor do something
		for (int x = 0; x < VIEW_WIDTH; x++) {
			for (int y = 0; y < VIEW_HEIGHT; y++) {
				pair<int, int> cur_position = { x, y };	//Store current position

				list<Actor*>::iterator it;
				for (it = myActors[x][y].begin(); it != myActors[x][y].end(); it++) {
					if ((*it)->isDead() == false) {	//If object is still alive, call its doSomething()
						(*it)->doSomething();

						if ((*it)->getX() != cur_position.first || (*it)->getY() != cur_position.second) {	//If the object changed position, store its pointer, old position, and new position
							old_pos.push_back(cur_position);
							new_pos.push_back({ (*it)->getX(),(*it)->getY() });
							object.push_back(*it);
						}
					}
				}

			}
		}

		for (size_t i = 0; i < object.size(); i++) {	//Shifts the objectts in the data structure from it's old position to its  new position
			myActors[old_pos[i].first][old_pos[i].second].remove(object[i]);
			myActors[new_pos[i].first][new_pos[i].second].push_back(object[i]);
		}

		updateDisplayText();

		removeDeadSimulationObjects();	//Delete any object that is dead
										//Remove dead objects

		return GWSTATUS_CONTINUE_GAME;
	}
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.

	if (m_ticks == 2000) {
		int winningAntNumber;
		for (int i = 0; i < getFilenamesOfAntPrograms().size(); i++) {
			int max = antsProduced[0];
			winningAntNumber = 0;
			if (antsProduced[i] >= max) {
				winningAntNumber = i;
				max = antsProduced[i];
			}
		}
		setWinner(antNames[winningAntNumber]);
		return GWSTATUS_PLAYER_WON;
	}


	return GWSTATUS_NO_WINNER;

}

void StudentWorld::cleanUp() {
	for (int x = 0; x < VIEW_WIDTH; x++) {
		for (int y = 0; y < VIEW_HEIGHT; y++) {
			list<Actor*>::iterator it;
			for (it = myActors[x][y].begin(); it != myActors[x][y].end(); it++) {
				delete *it;
			}
			myActors[x][y].clear();
		}
	}

}

void StudentWorld::createNewObject(int x, int y, string className, int health, int colony) {	//Creates a new object at a location
	if (className == "Adult") {
		myActors[x][y].push_back(new Adult(this, x, y));
	}
	else if (className == "Food") {
		bool foundFood = false;
		list<Actor*>::iterator it = myActors[x][y].begin();
		while (it != myActors[x][y].end() && !foundFood) {
			if ((*it)->getEdibility()) {
				(*it)->gainHealth(health);
				foundFood = true;
			}
			it++;
		}

		if (foundFood)
			return;
		else {
			if (it == myActors[x][y].end())
				myActors[x][y].push_back(new Food(this, x, y, health));
		}

		/*	list<Actor*>::iterator it;
		for ( it = myActors[x][y].begin(); it != myActors[x][y].end(); it++) {
		if ((*it)->getFoodID() == "IID_FOOD") {
		(*it)->gainHealth(100);
		foundFood = true;
		break;
		}
		}
		if (foundFood)
		return;
		else (it == myActors[x][y].end()) {
		myActors[x][y].push_back(new Food(this, x, y, 100));
		}
		*/
	}
	else if (className == "Pheromones") {
		bool foundPheromone = false;
		list<Actor*>::iterator it = myActors[x][y].begin();
		while (it != myActors[x][y].end() && !foundPheromone) {
			if ((*it)->isPheromone() && !(*it)->isEnemy(colony)) {
				(*it)->gainHealth(health);
				if ((*it)->getHealth() >= 768) {
					int excess = (*it)->getHealth() - 768;
					(*it)->loseHealth(excess);
				}
				foundPheromone = true;
			}
			it++;
		}

		if (foundPheromone)
			return;
		else {
			if (it == myActors[x][y].end())
				myActors[x][y].push_back(new Pheromones(this, IID_PHEROMONE_TYPE0 + colony, x, y, 256, colony));
			return;
		}

		//	Pheromones(StudentWorld* s, int imageID, int x, int y, int hitpoints, int colony) : HitpointObject(s, imageID, x, y, 256, right, 0.25, 2), m_colony(colony) {}
	}
	return;
}

void StudentWorld::removeDeadSimulationObjects() {
	while (deadCoordinates.empty() == false) {	//Goes through stack of dead objects
		pair<int, int> currCoord = deadCoordinates.top();
		Actor* currActor = deadActors.top();
		list<Actor*>::iterator a, b, itr;
		a = myActors[currCoord.first][currCoord.second].begin();
		b = myActors[currCoord.first][currCoord.second].end();
		itr = find(a, b, currActor);

		if (itr == b)
			return;
		else {
			Actor* temp = *itr;
			myActors[currCoord.first][currCoord.second].erase(itr);
			delete temp;
		}

		deadCoordinates.pop();
		deadActors.pop();
	}

}

void StudentWorld::takeHealthFromInsect(int a, int x, int y, int damage, Actor* b) {
	list<Actor*>::iterator it;
	unsigned N = a;
	if (myActors[x][y].size() > N)
	{
		it = myActors[x][y].begin();
		advance(it, N);
		if ((*it)->getBitable()) {
			(*it)->loseHealth(damage);
			(*it)->setJustBit(true);
			if ((*it)->getIsGrasshopper() && (*it)->getJustBit()) {
				int retaliate = randInt(0, 1);
				if (retaliate == 0) {
					int pickInsect = randInt(0, myActors[x][y].size() - 1);	//if more than one insect randomly choose one to bite
					list<Actor*>::iterator itr;
					unsigned N = pickInsect;
					if (myActors[x][y].size() > N)
					{
						itr = myActors[x][y].begin();
						advance(itr, N);
						(*itr)->loseHealth(damage);
					}
				}
				else return;
			}
		}
		return;
	}
	else return;
}

void StudentWorld::poisonObjectsOnSquare(int x, int y) {
	list<Actor*>::iterator it;

	for (it = myActors[x][y].begin(); it != myActors[x][y].end(); it++) {
		if ((*it)->getPoisonability() && (*it)->getJustPoisoned() == false) {
			(*it)->loseHealth(150);
			(*it)->setJustPoisoned(true);
		}
	}

}

void StudentWorld::stunObjectsOnSquare(int x, int y) {	//Stuns any stunnable objects on the square
														//NEED A WAY TO ACCESS FUNCTIONS OF DERIVED CLASSES OF ACTOR
	list<Actor*>::iterator it;

	for (it = myActors[x][y].begin(); it != myActors[x][y].end(); it++) {
		if ((*it)->getStunability() && (*it)->getJustStunned() == false) {
			(*it)->setSleep(true);
			(*it)->addSleepTicks(2);
			(*it)->setJustStunned(true);
		}
	}

}

Actor* StudentWorld::getActorAtProposedLocation(int x, int y) {
	if (myActors[x][y].size() != 0)
		return myActors[x][y].front();
	else
		return nullptr;
}

bool StudentWorld::isPebble(int x, int y) {
	list<Actor*>::iterator it;

	for (it = myActors[x][y].begin(); it != myActors[x][y].end(); it++) {
		if ((*it)->doIBlock()) {
			cerr << (*it)->getID() << endl;
			return true;
		}
		else
			return false;
	}
	return false;
}
bool StudentWorld::isNextPebble(int x, int y) {	//Checks to see if the object in the next square is a pebble
	Actor* ap = getActorAtProposedLocation(x, y);
	if (ap != nullptr)
	{
		Pebble* pp = dynamic_cast<Pebble*>(ap);
		if (pp != nullptr)
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

Actor* StudentWorld::checkSpace(int x, int y, string search) {
	list<Actor*> actors = getActorList(x, y);
	for (auto actor : actors) {
		if (search == "Pebble") {
			Pebble* pp = dynamic_cast<Pebble*>(actor);
			if (pp != nullptr) {
				return pp;
			}
		}
		else if (search == "Food") {
			Food* f = dynamic_cast<Food*>(actor);
			if (f != nullptr) {
				return f;
			}
		}
		else if (search == "Adult") {
			Adult* a = dynamic_cast<Adult*>(actor);
			if (a != nullptr) {
				return a;
			}
		}
		else if (search == "Pheromones") {
			Pheromones* p = dynamic_cast<Pheromones*>(actor);
			if (p != nullptr) {
				return p;
			}
		}
		else {
			return actor;
		}
	}
	return nullptr;
}

bool StudentWorld::isThereFood(int x, int y) {	//Checks if the square has food in it
	Actor* ap = getActorAtProposedLocation(x, y);
	if (ap != nullptr)
	{
		Food* pp = dynamic_cast<Food*>(ap);
		if (pp != nullptr)
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool StudentWorld::isThereAnthill(int x, int y, int colony) {	//Checks if the square has food in it
	Actor* ap = getActorAtProposedLocation(x, y);
	if (ap != nullptr)
	{
		Anthill* pp = dynamic_cast<Anthill*>(ap);
		if (pp != nullptr)
		{
			if (pp->getColony() == colony)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}


bool StudentWorld::isTherePheromone(int x, int y) {	//Checks if the square has food in it
	Actor* ap = getActorAtProposedLocation(x, y);
	if (ap != nullptr)
	{
		Pheromones* pp = dynamic_cast<Pheromones*>(ap);
		if (pp != nullptr)
		{
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

int StudentWorld::loadTheField(Field &f) {
	std::string fieldFile = getFieldFilename();		//CHANGE BACK TO GETFIELDNAME()

	std::string error;
	if (f.loadField(fieldFile, error) != Field::LoadResult::load_success)
	{
		setError(fieldFile + " " + error);
		return GWSTATUS_LEVEL_ERROR; // something bad happened!
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::updateDisplayText() {

	int ticks = getCurrentTicks();

	int antsAnt0, antsAnt1, antsAnt2, antsAnt3;
	int winningAntNumber;
	antsAnt0 = antsProduced[0];
	antsAnt1 = antsProduced[1];
	antsAnt2 = antsProduced[2];
	antsAnt3 = antsProduced[3];

	for (size_t i = 0; i < getFilenamesOfAntPrograms().size(); i++) {
		int max = antsProduced[0];
		winningAntNumber = 0;
		if (antsProduced[i] >= winningAntNumber) {
			winningAntNumber = i;
			max = antsProduced[i];
		}
	}

	// Create a string from your statistics, of the form:
	// Ticks: 1134 - AmyAnt: 32 BillyAnt: 33 SuzieAnt*: 77 IgorAnt: 05
	string s = formatThingsNicely(ticks, antsAnt0, antsAnt1, antsAnt2, antsAnt3, winningAntNumber);
	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string StudentWorld::formatThingsNicely(int ticks, int first, int second, int third, int fourth, int winning) {
	ostringstream oss;
	oss << "Ticks:";
	oss << setw(5) << ticks << " - ";
	if (getFilenamesOfAntPrograms().size() == 4) {
		if (winning == 0)
			oss << antNames[0] << "*: " << first << "  ";
		else
			oss << antNames[0] << ": " << first << "  ";

		if (winning == 1)
			oss << antNames[1] << "*: " << second << "  ";
		else
			oss << antNames[1] << ": " << second << "  ";

		if (winning == 2)
			oss << antNames[2] << "*: " << third << "  ";
		else
			oss << antNames[2] << ": " << third << "  ";

		if (winning == 3)
			oss << antNames[3] << "*: " << fourth << "  ";
		else
			oss << antNames[3] << ": " << fourth << "  ";
		string s = oss.str();

		return s;
	}
	else if (getFilenamesOfAntPrograms().size() == 3) {
		if (winning == 0)
			oss << antNames[0] << "*: " << first << "  ";
		else
			oss << antNames[0] << ": " << first << "  ";

		if (winning == 1)
			oss << antNames[1] << "*: " << second << "  ";
		else
			oss << antNames[1] << ": " << second << "  ";

		if (winning == 2)
			oss << antNames[2] << "*: " << third << "  ";
		else
			oss << antNames[2] << ": " << third << "  ";

		string s = oss.str();

		return s;
	}
	else if (getFilenamesOfAntPrograms().size() == 2) {
		if (winning == 0)
			oss << antNames[0] << "*: " << first << "  ";
		else
			oss << antNames[0] << ": " << first << "  ";

		if (winning == 1)
			oss << antNames[1] << "*: " << second << "  ";
		else
			oss << antNames[1] << ": " << second << "  ";

		string s = oss.str();

		return s;
	}
	else if (getFilenamesOfAntPrograms().size() == 1) {
		if (winning == 0)
			oss << antNames[0] << "*: " << first << "  ";
		else
			oss << antNames[0] << ": " << first << "  ";


		string s = oss.str();

		return s;
	}
	else {
		string s = oss.str();
		return s;
	}
}