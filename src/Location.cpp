#include <stdio.h>
#include "Location.h"
#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/Point.h"
#include "repast_hpc/Random.h"

Location::Location(repast::AgentId FieldID, repast::AgentId* houseID, int locationstate, char z, bool iswater,  int pstHarvest, int expHarvest){
	LocationID = FieldID;
	householdID = houseID;
	state = locationstate;
	zone = z;
	isWater = iswater;
	presentHarvest = pstHarvest;
	expectedHarvest = expHarvest;

}//for initialisaiton

Location::Location(repast::AgentId FieldID, int locationstate, char z, bool iswater,  int pstHarvest, int expHarvest){
	LocationID = FieldID;
	state = locationstate;
	zone = z;
	isWater = iswater;
	presentHarvest = pstHarvest;
	expectedHarvest = expHarvest;
}

Location::Location(){}
Location::~Location() {}

bool Location::getWater(){
	return isWater;
}

//get expected field harvest
int Location::getExpectedYield(){
    return expectedHarvest;
}

//calculate harvest of the field
void Location::calculateYield(int y, int q, double Ha){
  //Need to add formula to calculate expected yield
    expectedHarvest = y * q * Ha;
}
