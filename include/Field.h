#include <repast_hpc/AgentId.h>
#include <SharedSpaces.h>
#inlcude "Location.h"

class Field::Location {

private:

	repast::AgentId fieldID;
	repast::AgentId householdID;
	int presentHarvest;
	int expectedHarvest;

public:

	Field(repast::AgentId fieldID); //for initialisaiton
	~Field(); //Deconstructor

	void getID();
	void getType();
	int getExpectedYield();
	void calculateYield();
	Field* assignField();

}

#endif