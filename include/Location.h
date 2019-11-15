#ifndef LOCATION
#define LOCATION

#include <repast_hpc/AgentId.h>
#include <repast_hpc/SharedDiscreteSpace.h>
#include "Location.h"

class Location{
private:
	repast::AgentId LocationID;
	repast::AgentId* householdID;
	int state;
	char zone;
	bool isWater;
	int presentHarvest;
	int expectedHarvest;

public:
	Location(repast::AgentId FieldID, repast::AgentId* houseID, int locationstate, char z, bool iswater,  int pstHarvest, int expHarvest); //for initialisaiton
	Location(repast::AgentId FieldID, int locationstate, char z, bool iswater,  int pstHarvest, int expHarvest);
	Location();
	~Location(); //Deconstructor

	virtual repast::AgentId& getId() { return LocationID; }
	virtual const repast::AgentId& getId() const { return LocationID; }
	bool getWater();
	int getExpectedYield();
	void calculateYield(int y, int q, double Ha);
	Location* assignField();

};

#endif
