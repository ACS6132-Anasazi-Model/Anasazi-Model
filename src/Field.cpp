#include <stdio.h>
#inlcude "Location.h"
#include "Field.h"
#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/Properties.h"

Field::Field(repast::AgentId fieldID, repast::AgentId householdID, int pstHarvest, int expHarvest) {
    expHarvest = 0; 
}
Field::~Field() {}

int Field::getpstYield(pstHarvest){
        return expHarvest;
}
//get expected field harvest
int Field::getExpYield(expHarvest){
    expHarvest = initAvgHarvest; //initialize the harvest of field
    if (getlocationstate->locationstate = 0){   //location is water
        return 0;
    }else if (getlocationstate->locationstate = 1) {  //location is household
        return 0;
    }else if (getlocationstate->locationstate = 2) {  //location is empty
        return 0;
    }else if (getlocationstate->locationstate = 3) {  //location is uninhabited
        return expHarvest = expHarvest;
    }else if (getlocationstate->locationstate = 4) {  //location is inhabited
        return expHarvest = expHarvest + householdmaizestorge->maizestorge();
        if(expHarvest > 1600){
            expHarvest = 1600;
        }
    }
}

//calculate harvest of the field
void Field::CalculateYield(){
   if (getlocationstate->locationstate = 0){   //location is water
        return 0;
    }else if (getlocationstate->locationstate = 1) {  //location is household
        return 0;
    }else if (getlocationstate->locationstate = 2) {  //location is empty
        return 0;
    }else if (getlocationstate->locationstate = 3) {  //location is uninhabited
        return expHarvest ();
    }else if (getlocationstate->locationstate = 4) {  //location is inhabited
        return expHarvest = expHarvest - 13.33*household->agentamount() + householdmaizestorge->maizestorge(); 
    }
}

