#ifndef HOUSEHOLD
#define HOUSEHOLD

#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/Random.h"
#include "Field.h"

class Household{
private:
	repast::AgentId householdId;
	Field* assignedField;
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

	/* Actions */
	void move(repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* space);
};

#endif
