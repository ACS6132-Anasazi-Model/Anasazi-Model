#include <stdio.h>
#inlcude "Location.h"
#include "Field.h"
#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/Properties.h"

Field::Field(repast::AgentId fID, repast::AgentId* houseID, int pstHarvest, int expHarvest) {
    fieldID = fID;
    householdId = houseID;
    presentHarvest = pstHarvest;
    expectedHarvest = expHarvest;
}
Field::~Field() {}

//get expected field harvest
int Field::getExpectedYield(){
  return expectedHarvest;
}

//calculate harvest of the field
void Field::CalculateYield(){
  //Need to add formula to calculate expected yield
}
