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


//get expected field harvest
int Location::getExpectedYield(){
    return expectedHarvest;
}

//calculate harvest of the field
void Location::calculateYield(int y, int q, double Ha){
  //Need to add formula to calculate expected yield
    expectedHarvest = y * q * Ha;
}

bool Location::getWater(int year, char zone, int watertype, int startdate, int enddate, int waterx, int watery){
	int existStreams = 0, existAlluvium = 0;
    bool watersource = 0;
    if (watertype == 1){
		if ((year >= 280 && year < 360) or (year >= 800 && year < 930) or (year >= 1300 && year < 1450)){
			existStreams = 1;
		}
		else{
			existStreams = 0;
		}
		if (((year >= 420) && (year < 560)) or ((year >= 630) && (year < 680)) or	((year >= 980) && (year < 1120)) or ((year >= 1180) && (year < 1230))){
			existAlluvium = 1;
		}
		else{
			existAlluvium = 0;
		}
		//for each location {
		
		if ((existAlluvium == 1) && ((zone == 'General') or (zone == 'North') or (zone == 'Mid') or (zone == 'Kinbiko'))) {
				watersource = 1;
			}
			else if ((existStreams == 1) && (zone == 'Kinbiko')) {
				watersource = 1;
			}
		//for these locations: (location 72 114) (location 70 113) (location 69 112)	(location 68 111) (location 67 110) (location 66 109) (location 65 108) (location 65 107)) 
		if (((waterx==72)&&(watery==114))or((waterx==70)&&(watery==113))or((waterx==69)&&(watery==112))or((waterx==68)&&(watery==111))or((waterx==67)&&(watery==110))or((waterx==66)&&(watery==109))or((waterx==65)&&(watery==108))or((waterx==65)&&(watery==107))){
			watersource = 1;
		}
	}
	else if (watertype == 2){
			watersource = 1; //the location with matching x,y coor has water source
	}
	else if (watertype == 3){
			if ((year >= startdate) && (year <= enddate)) {
				watersource = 1;
			}
	}
	return watersource;
}