#include <stdio.h>
#include "Location.h"
#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/Point.h"
#include "repast_hpc/Random.h"

Location::Location(repast::AgentId FieldID){
	LocationID = FieldID;
}//for initialisaiton

Location::Location(){}
Location::~Location() {}

void Location::setZones(int z, int mz){
	zone = z;
	maizeZone = mz;
}


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
