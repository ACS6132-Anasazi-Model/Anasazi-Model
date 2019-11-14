#ifndef FIELD
#define FIELD

#include <repast_hpc/AgentId.h>
#include <repast_hpc/SharedDiscreteSpace.h>
#include "Location.h"

class Field: public Location {
private:
	repast::AgentId* householdID;
	int presentHarvest;
	int expectedHarvest;

public:
	Field(repast::AgentId fID, repast::AgentId* houseID, int pstHarvest, int expHarvest); //for initialisaiton
	~Field(); //Deconstructor

	int getExpectedYield();
	void calculateYield(int y, int q, double Ha);
	Field* assignField();
};

#endif
