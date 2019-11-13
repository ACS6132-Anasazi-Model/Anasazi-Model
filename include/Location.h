#ifndef LOCATION
#define LOCATION

#include "repast_hpc/SharedDiscreteSpace.h"

class Location{
protected:
	repast::AgentId LocationID;
	int state;
	char zone;
	bool isWater;

public:
	Location(repast::AgentId LocationID, int locationstate, char zone, bool iswater); //for initialisaiton
	Location();
	~Location(); //Deconstructor

	virtual repast::AgentId& getId() { return LocationID; }
	virtual const repast::AgentId& getId() const { return LocationID; }
	bool getWater();

};

#endif
