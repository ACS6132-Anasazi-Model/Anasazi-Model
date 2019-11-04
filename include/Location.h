#include <SharedSpaces.h>

class Location{

private:

	int state;
	char locationType;
	char zone;

public:

	Location(); //for initialisaiton
	~Location(); //Deconstructor

	void getState();
	void getType();
	void getZone();

}