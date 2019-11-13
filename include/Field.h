#include <repast_hpc/AgentId.h>
#include <SharedSpaces.h>
#inlcude "Location.h"

class Field::Location {

private:

	repast::AgentId* householdID;
	int presentHarvest;
	int expectedHarvest;

public:

	Field(repast::AgentId fieldID, repast::AgentId* householdID, int presentHarvest, int expectedHarvest); //for initialisaiton
	~Field(); //Deconstructor

	int getExpectedYield();
	void calculateYield();
	Field* assignField();

}

#endif
