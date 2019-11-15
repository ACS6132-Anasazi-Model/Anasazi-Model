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
	int zone;
	int maizeZone;
	bool isWater;
	int presentHarvest;
	int expectedHarvest;

public:
	Location(repast::AgentId FieldID); //for initialisaiton
	Location();
	~Location(); //Deconstructor

	void setZones(int z, int mz);

	virtual repast::AgentId& getId() { return LocationID; }
	virtual const repast::AgentId& getId() const { return LocationID; }
	bool getWater();
	int getExpectedYield();
	void calculateYield(int y, int q, double Ha);
	Location* assignField();

};

#endif
