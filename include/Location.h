#ifndef LOCATION
#define LOCATION

#include <repast_hpc/AgentId.h>
#include <repast_hpc/SharedDiscreteSpace.h>
#include "Location.h"
#include <vector>

class Location{
private:
	repast::AgentId LocationID;
	repast::AgentId* householdID;
	int state;
	int zone;
	/*Empty 		- 0
	Natural 		- 1
	Kinbiko 		- 2
	Uplands 		- 3
	North 			- 4
	General 		- 5
	North Dunes - 6
	Mid Dunes 	- 7
	Mid 				- 8*/
	int maizeZone;
	/*Empty 		- 0
		No_Yield 	- 1
		Yield_1		- 2 || North and Mid Valley, Kinbiko Canyon
		Yield_2 	- 3 || General Valley
		Yield_3		- 4 || Arable Uplands
		Sand_dune - 5 || Dunes */
	bool isWater;

	struct WaterSource
	{
		int waterType;
		int startYear;
		int endYear;
		bool isWater;
	};
	std::vector<WaterSource> waterSources;

	int presentHarvest;
	int expectedHarvest;


public:
	Location(repast::AgentId FieldID); //for initialisaiton
	Location();
	~Location(); //Deconstructor

	void setZones(int z, int mz);
	void addWaterSource(int waterType, int startYear, int endYear);

	virtual repast::AgentId& getId() { return LocationID; }
	virtual const repast::AgentId& getId() const { return LocationID; }

	void checkWater(bool existStreams, bool existAlluvium, int x, int y, int year);
	int getExpectedYield();
	void calculateYield(int y, int q, double Ha);
	Location* assignField();

};

#endif
