#ifndef HOUSEHOLD
#define HOUSEHOLD

#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/Random.h"
#include "Location.h"
#define DEBUG

class Household{
private:
	repast::AgentId householdId;
	Location* assignedField;
  int maizeStorage;
	int age;
	int deathAge;

public:
	Household(repast::AgentId id,int a, int deathAge, int mStorage); //for init
	~Household();

	/* Required Getters */
	virtual repast::AgentId& getId() { return householdId; }
	virtual const repast::AgentId& getId() const { return householdId; }
	Location* getAssignedField(){return assignedField; }

	/* Getters specific to this kind of Agent */
	int splitMaizeStored(int percentage);
  bool checkMaize();
  bool death();
  bool fission(int minFissionAge, int maxFissionAge, double gen);
	void nextYear();
	void chooseField(Location* Field);

	/* Getters for test1 */
	int getMaizeStorage(){ return maizeStorage; }
	int getAge() { return age; }
	int getDeathAge() { return deathAge; }


	/* Actions */
	void move(repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* space);
};

#endif
