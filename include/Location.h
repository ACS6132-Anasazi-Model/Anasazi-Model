#include <SharedSpaces.h>

class Location{

private:

	repast::AgentId LocationID;
	int state;
	char zone;
	bool isWater;

public:

	Location(); //for initialisaiton
	~Location(); //Deconstructor

	repast::AgentId getID();
	bool getWater();

}
