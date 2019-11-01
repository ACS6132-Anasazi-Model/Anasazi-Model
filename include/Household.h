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
	Household(repast::AgentId id, int HouseholdType, double threshold); //for init
	~Household();
	
	/* Required Getters */
	virtual repast::AgentId& getId() { return HouseholdId; }
	virtual const repast::AgentId& getId() const { return HouseholdId; }
	
	/* Getters specific to this kind of Agent */
	int getType() 
    int getMaizeStorge()
	bool getMaizeStatus() 
    bool getFissionStatus() 
    bool getFertilityStatus() 
	
	/* Actions */
	void updateStatus(repast::SharedContext<Household>* context,
			  repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* space);
	void move(repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* space);	
};

#endif