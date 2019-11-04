#include <repast_hpc/AgentId.h>
#include <SharedSpaces.h>
#inlcude "Location.h"

class Field::Location {

private:

	repast::AgentId fieldID;
	repast::AgentId householdID;
	int pstHarvest;
	int expHarvest;
	void calYield();
	void assignField();

public:

	Field(repast::AgentId fieldID); //for initialisaiton
	~Field(); //Deconstructor

	void getID();
	void getType();

}

#endif