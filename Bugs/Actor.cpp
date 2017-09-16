#include "Actor.h"
#include "StudentWorld.h"
#include <stdlib.h>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
GraphObject::Direction Actor::generateRandomDirection() {	//generates a random direction
	int direction = randInt(0, 3);
	switch (direction) {
	case 0:
		return up;
		break;
	case 1:
		return right;
		break;
	case 2:
		return down;
		break;
	case 3:
		return left;
		break;
	default:
		cerr << "Error. No Random Direction Created" << endl;
		return none;
		break;
	}
}

void Water::doSomething() {
	getWorld()->stunObjectsOnSquare(getX(), getY());
}

void Poison::doSomething() {
	getWorld()->poisonObjectsOnSquare(getX(), getY());
}

void HitpointObject::moveDistanceInDirection(int x, int y, int antOrNot) {	//antOrNot will be 0 if not an ant, 1 if it is an ant
	if (getInstructions().second <= 0) {	//If object has finished travelling distance, generate random direction and distance (new instructions)
		if (antOrNot == 0) {
			Direction randDir = generateRandomDirection();
			int distance = randInt(2, 10);
			setInstructions(randDir, distance);
		}
		else {
			int distance = 1;
			setInstructions(getDirection(), distance);
		}
	}
	switch (m_instructions.first) {
	case up:	//Move object up
		setDirection(up);
		if (getWorld()->isNextPebble(x, y + 1)) {	//Checks if next object is a pebble
			m_instructions.second = 0;
			setMoveBlock(true);
			break;
		}
		moveTo(x, y + 1);
		m_instructions.second--;
		break;

	case right:	//Move object right
		setDirection(right);
		if (getWorld()->isNextPebble(x + 1, y)) {
			m_instructions.second = 0;
			setMoveBlock(true);
			break;
		}
		moveTo(x + 1, y);
		m_instructions.second--;

		break;

	case down:	//Move object down
		setDirection(down);

		if (getWorld()->isNextPebble(x, y - 1)) {
			m_instructions.second = 0;
			setMoveBlock(true);
			break;
		}
		moveTo(x, y - 1);
		m_instructions.second--;

		break;

	case left:	//Move object left
		setDirection(left);
		if (getWorld()->isNextPebble(x - 1, y)) {
			m_instructions.second = 0;
			setMoveBlock(true);
			break;
		}
		moveTo(x - 1, y);
		m_instructions.second--;
		break;

	default:
		cerr << "ERROR. NO DIRECTION SPECIFIED" << endl;
		return;
	}
}

void HitpointObject::makeDead(string name) {	//Sets object dead
	if (name == "Food" || name == "Pheromones" || name == "Anthill") {	//If the object that died was a food item, only make it dead.  Don't generate a new item
		getWorld()->pushCoordStack(getX(), getY());
		getWorld()->pushActorStack(this);
		setDead();
	}
	else {	//Any other item other than a food item will create a new food item
		getWorld()->pushCoordStack(getX(), getY());
		getWorld()->pushActorStack(this);
		setDead();
		getWorld()->createNewObject(getX(), getY(), "Food", 100, 4);
	}

	return;

}

void HitpointObject::makeDead(Actor* a) {	//For deleting specific pointers
	getWorld()->pushCoordStack(getX(), getY());
	getWorld()->pushActorStack(a);
	setDead();
	return;
}

void Insect::checkIfSleep() {	//Checks if object is sleeping/stunned
	if (isSleep()) {	//If it is, decrease the sleepticks by 1
		decreaseSleep();
		if (getSleepTicks() == -1) {	//After two ticks (default) of sleeping, wake up object
			setSleep(false);
			setSleepTicks(0);
			return;
		}
		return;
	}
}

void Insect::eat() {
	if (getWorld()->isThereFood(getX(), getY())) {
		Actor* foodPointer = getWorld()->checkSpace(getX(), getY(), "Food");
		if (foodPointer->getHealth() <= 200) {
			makeDead(foodPointer);
			gainHealth(foodPointer->getHealth());
		}
		else {
			foodPointer->loseHealth(200);
			gainHealth(200);
		}
		setEatStatus(true);
	}
}

void Insect::bite(int damage, int colony) {
	list<Actor*> listAtPos;
	vector<Actor*> vectorAtPos;
	listAtPos = getWorld()->getActorList(getX(), getY());

	list<Actor*>::iterator it;
	for (it = listAtPos.begin(); it != listAtPos.end(); it++) {
		if ((*it)->getBitable() && (*it)->isEnemy(colony)) {
			vectorAtPos.push_back(*it);
		}
	}

	//	int biteOrNot = randInt(0, 2);//1/3 chance of biting insect on same square
	if (vectorAtPos.size() > 0) {
		//		if (biteOrNot == 0) {
		int pickInsect = randInt(0, vectorAtPos.size() - 1);	//if more than one insect randomly choose one to bite
		vectorAtPos[pickInsect]->loseHealth(damage);
		vectorAtPos[pickInsect]->setJustBit(true);
		if (vectorAtPos[pickInsect]->getIsGrasshopper()) {
			int retaliate = randInt(0, 1);
			if (retaliate == 0) {
				loseHealth(50);
			}
		}
		setSleep(true);
		setSleepTicks(2);//Set the grasshopper to sleep for two ticks
		return;
		//		}
		//		else return;
	}
	else return;
}

void Baby::doSomething() {
	//MOVE IMPLEMENTATION
	loseHealthOnTick();

	if (getHealth() <= 0)	//Check if object has lost all it's health
		makeDead();

	if (isDead()) return;

	checkIfSleep();	//Checks if object is sleeping or stunned

	if (isSleep()) return;

	if (getHealth() >= 1600) {	//If baby has greater than 1600 healthpoints, then it will transform
		getWorld()->createNewObject(getX(), getY(), "Adult", 1600, 4);	//Create a new adult grasshoppers
		makeDead();
		return;
	}

	if (isDead()) return;

	setJustStunned(false);	//If object moved from pool of water, then it can be stunned again

	eat();
	setMoveBlock(false);
	int restOrNot;

	if (getEatStatus() == true)	//check if object has ate
		restOrNot = randInt(0, 1);

	int currentX = getX();
	int currentY = getY();
	int currentDir = getDirection();

	if (getEatStatus()) {	//If object has just ate, there is a 50/50 chance of the object either resting or moving
		if (restOrNot == 0) //Rest
			moveDistanceInDirection(currentX, currentY, 0);
		else if (restOrNot == 1) {	//Sleep
			setSleep(true);
			setSleepTicks(2);
		}
		setEatStatus(false);
	}
	else	//If object did not eat, just move the object
		moveDistanceInDirection(currentX, currentY, 0);

	setSleep(true);	//object sleeps after moving for 2 ticks
	setSleepTicks(2);
}

void Adult::generateRadius(int& finalX, int& finalY) {
	int r = randInt(1, 10);
	double pi = 3.14159265358979323846;
	int degrees = randInt(0, 360);
	double theta = degrees * pi / 180;
	int newX = getX() + r * cos(theta);
	int newY = getY() + r * sin(theta);

	finalX = newX;
	finalY = newY;
}

void Adult::jump() {
	int newX = 0;
	int newY = 0;
	int jumpOrNot = randInt(0, 9);

	if (jumpOrNot == 0) {
		int i = 0;
		while (i != 100) {
			generateRadius(newX, newY);
			if (/*!getWorld()->isPebble(newX, newY) && */newX < VIEW_WIDTH -1 && newX > 0 && newY < VIEW_HEIGHT -1 && newY > 0) {
			//	cerr << newX << " , " << newY << endl;
				moveTo(newX, newY);
				setSleep(true);	//object sleeps after moving for 2 ticks
				setSleepTicks(2);
				break;
				return;
			}
			i++;
		}
		return;
	}
	else
		return;
}

void Adult::doSomething() {
	//MOVE IMPLEMENTATION
	loseHealthOnTick();

	if (getHealth() <= 0)	//Check if object has lost all it's health
		makeDead("asef");

	if (isDead()) return;

	checkIfSleep();	//Checks if object is sleeping or stunned
	if (isSleep()) return;

	setSleep(false);
	setSleepTicks(0);

	bite(50, 4);	//Need to test if it is bitable //DOES NOT WORK PROPERLY.  MAKES GRASSHOPPERS STOP AFTER ABOUT 600 TICKS

	if (isSleep()) return;

	setSleep(false);
	setSleepTicks(0);

	jump();

	if (isSleep()) return;

	setSleep(false);
	setSleepTicks(0);

	setJustPoisoned(false);

	eat();
	setMoveBlock(false);
	int restOrNot;

	if (getEatStatus() == true)	//check if object has ate
		restOrNot = randInt(0, 1);

	int currentX = getX();
	int currentY = getY();
	int currentDir = getDirection();

	if (getEatStatus()) {	//If object has just ate, there is a 50/50 chance of the object either resting or moving
		if (restOrNot == 0) //Rest
			moveDistanceInDirection(currentX, currentY, 0);
		else if (restOrNot == 1) {	//Sleep
			setSleep(true);
			setSleepTicks(2);
		}
		setEatStatus(false);
	}
	else	//If object did not eat, just move the object
		moveDistanceInDirection(currentX, currentY, 0);

	if (isSleep()) return;

	setSleep(true);	//object sleeps after moving for 2 ticks
	setSleepTicks(2);
}

void Pheromones::doSomething() {
	loseHealthOnTick();

	if (getHealth() <= 0)	//Check if object has lost all it's health
		makeDead("Pheromones");

	if (isDead()) return;
}

void Anthill::doSomething() {
	loseHealthOnTick();
//	cout << getHealth() << endl;
	setEatStatus(false);

	if (getHealth() <= 0)	//Check if object has lost all it's health
		makeDead("Anthill");

	if (isDead()) return;

	//Check to see if there is any food on square
	//Gainhealth();
	//set justAte to true
	//setEatStatus(true);
	//return 
	if (getEatStatus())
		return;

	//Check if enough energy to create new ant
	if (getHealth() >= 2000) {
		getWorld()->setActorList(getX(), getY(), new Ant(getWorld(), IID_ANT_TYPE0 + getColony(), getX(), getY(), getColony(), new Compiler(getCompiler())));
		getWorld()->setAntsProduced(getColony());
		loseHealth(1500);
		//Ask studentWorld to increase the number of ants the colony has produced
	}
}

void Ant::doSomething() {
	loseHealthOnTick();

	setEatStatus(false);

	if (getHealth() <= 0)	//Check if object has lost all it's health
		makeDead("asef");

	if (isDead()) return;

	checkIfSleep();	//Checks if object is sleeping or stunned
	if (isSleep()) return;

	setSleep(false);
	setSleepTicks(0);

	setMoveBlock(false);
	setJustPoisoned(false);
	setJustBit(false);

//	moveDistanceInDirection(getX(), getY(), 1);
	Compiler::Command cmd;
//	cout << getHealth() << endl;
	int i = 0;
//	ic = 0;
	while (i != 10)
	{
		// get the command from element ic of the vector
		if (!getCompiler()->getCommand(ic, cmd)) {
			makeDead("asef");
			return;
		}
	//	cout << cmd.opcode << endl;
		switch (cmd.opcode)
		{
		case Compiler::moveForward:
			// cause the ant to move forward by
			// updating its x,y coordinates
			moveDistanceInDirection(getX(), getY(), 1);
			++ic; // advance to next instruction
		//	ic = 0;
			return;
			break;
		case Compiler::eatFood:
			if (m_foodCarry == 0) {
				++ic;
				return;
				break;
			}
			else if (m_foodCarry <= 100) {
				gainHealth(m_foodCarry);
				m_foodCarry = 0;
				++ic;
				return;
				break;
			}
			else {
				m_foodCarry -= 100;
			}
			++ic; // advance to next instruction
			return;
			break;
		case Compiler::dropFood:
		{
			int foodAmount = m_foodCarry;
			getWorld()->createNewObject(getX(), getY(), "Food", foodAmount, getColony());
			++ic;
			return;
			break;
		}
		case Compiler::bite:
			bite(15, getColony());
			++ic;
			return;
			break;
		case Compiler::pickupFood:
			if (getWorld()->isThereFood(getX(), getY())) {
				Actor* foodPointer = getWorld()->checkSpace(getX(), getY(), "Food");
				if (foodPointer->getHealth() <= 400) {
					makeDead(foodPointer);
					addFoodCarry(foodPointer->getHealth());
				}
				else {
					foodPointer->loseHealth(400);
					addFoodCarry(400);
				}
			}

			if (m_foodCarry >= 1800) {
				m_foodCarry = 1800;
			}
			++ic;
			return;
			break;
		case Compiler::emitPheromone:
			getWorld()->createNewObject(getX(), getY(), "Pheromones", 256, getColony());
			++ic;
			return;
			break;

		case Compiler::faceRandomDirection:
			setDirection(generateRandomDirection());
			++ic;
			return;
			break;


		case Compiler::generateRandomNumber:
		{
			if (stoi(cmd.operand1) == 0) {
				++ic;
				return;
				break;
			}
			int newRand = randInt(0, stoi(cmd.operand1) - 1);
			setRandom(newRand);
			++ic; // advance to next instruction
			return;
			break;
		}
		case Compiler::goto_command:
			ic = stoi(cmd.operand1);
			break;

		case Compiler::if_command:
			// if the condition of the if command is
			// is true, then go to the target position
			// in the vector; otherwise fall through to
			// the next position
			switch (stoi(cmd.operand1)) {
			case Compiler::last_random_number_was_zero:
				if (getRandom() == 0)
					ic = stoi(cmd.operand2);
				else
					++ic;
				break;
			case Compiler::i_am_carrying_food:
				if (m_foodCarry >= 0)
					ic = stoi(cmd.operand2);
				else
					++ic;
				break;
			case Compiler::i_am_hungry:
				if (getHealth() <= 25) 
					ic = stoi(cmd.operand2);
				else
					++ic;
				break;
			case Compiler::i_am_standing_with_an_enemy:
				++ic;
				break;
			case Compiler::i_am_standing_on_food:
				if (getWorld()->isThereFood(getX(), getY())) {
					Actor* foodPointer = getWorld()->checkSpace(getX(), getY(), "Food");
					if (foodPointer->getHealth() >= 1) {
						ic = stoi(cmd.operand2);
					}
					else
						++ic;
				}
				else
					++ic;
				break;
			case Compiler::i_am_standing_on_my_anthill:
				if (getWorld()->isThereAnthill(getX(), getY(), getColony())) {
					ic = stoi(cmd.operand2);
				}
				else
					++ic;
				break;
			case Compiler::i_smell_pheromone_in_front_of_me:
			{
					Direction direction = getDirection();
					switch (direction) {
					case up:
						if (getWorld()->isTherePheromone(getX(), getY() + 1)) {
							Actor* pPointer = getWorld()->checkSpace(getX(), getY(), "Pheromones");
							if (pPointer->getHealth() >= 1) {
								ic = stoi(cmd.operand2);
							}
						}
						else
							++ic;
						break;
					case right:
						if (getWorld()->isTherePheromone(getX() + 1, getY())) {
							Actor* pPointer = getWorld()->checkSpace(getX(), getY(), "Pheromones");
							if (pPointer->getHealth() >= 1) {
								ic = stoi(cmd.operand2);
							}
						}
						else
							++ic;
						break;
					case down:
						if (getWorld()->isTherePheromone(getX(), getY() - 1)) {
							Actor* pPointer = getWorld()->checkSpace(getX(), getY(), "Pheromones");
							if (pPointer->getHealth() >= 1) {
								ic = stoi(cmd.operand2);
							}
							else
								++ic;
						}
						else
							++ic;
						break;
					case left:
						if (getWorld()->isTherePheromone(getX() - 1, getY())) {
							Actor* pPointer = getWorld()->checkSpace(getX(), getY(), "Pheromones");
							if (pPointer->getHealth() >= 1) {
								ic = stoi(cmd.operand2);
							}
							else
								++ic;
						}
						else
							++ic;
						break;
					default:
						break;
					} 
				break;
			}
			case Compiler::i_smell_danger_in_front_of_me:
			{
				vector<Actor*> vectorAtPos;
				list<Actor*> listAtPos = getWorld()->getActorList(getX(), getY());

				list<Actor*>::iterator it;
				for (it = listAtPos.begin(); it != listAtPos.end(); it++) {
					if ((*it)->getBitable() && (*it)->isEnemy(getColony())) {
						vectorAtPos.push_back(*it);
					}
				}

				if (vectorAtPos.size() > 0)
					ic = stoi(cmd.operand2);
				else
					++ic;
				break;
			}
			case Compiler::i_was_bit:
				if (getJustBit())
					ic = stoi(cmd.operand2);	
				else
					++ic;
				break;
			case Compiler::i_was_blocked_from_moving:
				if (getMoveBlock())
					ic = stoi(cmd.operand2);
				else
					++ic;
				break;
			default:
				++ic;
				return;
			}
			
		}
		
		i++;
	}
	
}
