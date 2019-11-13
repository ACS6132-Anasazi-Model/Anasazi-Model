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

repast::AgentId getID()
{
	return LocationID;
}

bool Location::getWater(){
	return isWater;
}
