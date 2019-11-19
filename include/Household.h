#ifndef HOUSEHOLD
#define HOUSEHOLD

#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/Random.h"
#include "Location.h"

class Household{
private:
	repast::AgentId householdId;
	Location* assignedField;
  int maizeStorge;
	bool maizeSatisfied;
  bool householdFission;
	int age;
	int deathAge;

public:
	Household(repast::AgentId id, int deathAge); //for init
	~Household();

	/* Required Getters */
	virtual repast::AgentId& getId() { return householdId; }
	virtual const repast::AgentId& getId() const { return householdId; }

	/* Getters specific to this kind of Agent */
  int checkMaize();
  bool death();
  bool fission(int fissionAge, int gen);
	void nextYear();
	repast::Point<int> chooseField();

	/* Getters for test1 */
	//AgentId getHouseholdId();
	//Location* getAssignedField();
	int getMaizeStorage(){ return maizeStorge; }
	bool getMaizeSatifieds() { return maizeSatisfied; }
	bool getHouseholdFission() {return householdFission; }
	int getAge() { return age; }
	int getDeathAge() { return deathAge; }


	/* Actions */
	void move(repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* space);
};

#endif
