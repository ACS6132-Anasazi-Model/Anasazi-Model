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

	virtual repast::AgentId& getId() { return LocationID; }
	virtual const repast::AgentId& getId() const { return LocationID; }
	bool getWater();

}
