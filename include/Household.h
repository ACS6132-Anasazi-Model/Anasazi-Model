#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"


class Household{
	
private:
	repast::AgentId HouseholdId;
	int HouseholdType;
    int MaizeStorge;
	bool MaizeSatisfied;
    bool HouseholdFission;
    bool Fertility;
	double threshold;
    

	
public:
	Agent(repast::AgentId id, int HouseholdType, double threshold); //for init
	~Agent();
	
	/* Required Getters */
	virtual repast::AgentId& getId() { return HouseholdId; }
	virtual const repast::AgentId& getId() const { return HouseholdId; }
	
	/* Getters specific to this kind of Agent */
	int getType() { return HouseholdType; }
    int getMaizeStorge;{ return MaizeStorge; }
	bool getMaizeStatus() { return MaizeSatisfied; }
    bool getFissionStatus() { return HouseholdFission; }
    bool getFertilityStatus() { return Fertility; }
	
	/* Actions */
	void updateStatus(repast::SharedContext<Agent>* context,
			  repast::SharedDiscreteSpace<Agent, repast::StrictBorders, repast::SimpleAdder<Agent> >* space);
	void move(repast::SharedDiscreteSpace<Agent, repast::StrictBorders, repast::SimpleAdder<Agent> >* space);	
};

#endif