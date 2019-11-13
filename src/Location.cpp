#include <stdio.h>
#inlcude "Location.h"
#include "Field.h"
#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/Point.h"
#include "repast_hpc/Random.h"

Location::Location(repast::AgentId LocationID, int locationstate, char LocationType, char zone, bool iswater) {}
Location::~Location() {}

repast::Point<double> origin(0,0);
repast::Point<double> extent(boardSize,boardSize);

repast::GridDimensions gd(origin, extent);
	
int procX = repast::strToInt(props->getProperty("proc.per.x"));
int procY = repast::strToInt(props->getProperty("proc.per.y"));
int bufferSize = repast::strToInt(props->getProperty("grid.buffer"));
 
std::vector<int> processDims;
processDims.push_back(procX);
processDims.push_back(procY);
discreteSpace = new repast::SharedDiscreteSpace<Agent, repast::StrictBorders, repast::SimpleAdder<Agent> >("AgentDiscreteSpace", gd, processDims, bufferSize, comm);

int Location::getlocationstate(locationstate){ //generate the location grid
    int rank = repast::RepastProcess::instance()->rank();
	repast::IntUniformGenerator gen = repast::Random::instance()->createUniIntGenerator(1, boardSize); //random from 1 to boardSize
	int locationstate0 = ; //water
	int locationstate1 = ; // household
    int locationstate2 = ; // empty
    int locationstate3 = ; // uninhabited
    int locationstate4 = ; // inhabited
	double threshold = repast::strToDouble(props->getProperty("threshold"));
	for (int i = 0; i <  ; i++){
		//random a location until an empty one is found (not the most efficient)
		int xRand, yRand;
		std::vector<water*> waterList;
		do {
			waterList.clear();
			xRand = gen.next(); yRand = gen.next();
			discreteSpace->getObjectsAt(repast::Point<int>(xRand, yRand), waterList);
		} while (waterList.size() != 0);

		//create household, assign type, move the agent to the randomised location
	/*	repast::Point<int> initialLocation(xRand, yRand);
		repast::AgentId LocationID(i, rank, 0);
		id.currentRank(rank);
		int type;
		if (countType0 > 0) {
			type = 0;
			countType0--;
		} else {
			type = 1;
			countType1--;
		}
		household* agent = new household(LocationID, type, threshold);
		context.addAgent(household);
		discreteSpace->moveTo(LocationID, initialLocation);*/
    locationstate = repast::Random::instance()->createUniIntGenerator(state);
}

bool Location::getwater(iswater){
    if(locationstate = 0){
        return 1;
    }else return 0;
}