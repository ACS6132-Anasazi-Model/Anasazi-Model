#include <boost/mpi.hpp>
#include "repast_hpc/RepastProcess.h"

#include "Model.h"
#include "Location.h"
#include "Household.h"


int main(int argc, char** argv){


	std::string configFile = argv[1]; // The name of the configuration file is Arg 1
	std::string propsFile  = argv[2]; // The name of the properties file is Arg 2

	boost::mpi::environment env(argc, argv);
	boost::mpi::communicator world;

	repast::RepastProcess::init(configFile);

	AnasaziModel* model = new AnasaziModel(propsFile, argc, argv, &world);
	/*
	repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();

	model->initAgents();
	model->initSchedule(runner);

	runner.run();

	delete model;

	repast::RepastProcess::instance()->done();
	*/


	int mode; //	select whether to run test 1-6, run the model or exit
	int deathAgetest; // set input age value
	int maizeYI; // set maize yield index
	int pseudoID=0;
	int i=0;

	//repast::AgentId id(pseudoID, 0, 0);
	//Household h1(id,0,0,0);

while(mode!=7){
	cout << "Select an option:" << endl;
	cout << "1. Test 1" << endl;
	cout << "2. Test 2" << endl;
	cout << "3. Test 3" << endl;
	cout << "4. Test 4" << endl;
	cout << "5. Test 5" << endl;
	cout << "6. Run Model" << endl;
	cout << "7. Exit" << endl;

	cin >> mode;

	switch(mode)
	{
	case 1:
		// run test1
		{
		cout << "Pick agent:" << endl;
		cin >> pseudoID;
		repast::AgentId id(pseudoID, 0, 0);
		Household h1(id,0,0,0);
		cout << h1.getId()  << endl;
		cout << "Age:" << h1.getAge()  << endl;
		cout << "Death Age:" << h1.getDeathAge()  << endl;
		cout << "Maize Storage:" << h1.getMaizeStorage()  << endl;
		cout << "Assigned Field:" << h1.getAssignedField() << endl;
		}
		break;
	case 2: // run test2
		cout << "Hello2" << endl;
		break;
	case 3: // run test3
		// compare the archaelogical data from water csv
		break;
	case 4: // run test4
		cout << "Set maize yield index:" << endl;
		cin >> maizeYI;
		// some function to set into properties files.
		break;
	case 5: // run test5 not wokring
		{
		cout << "set new death age" << endl;
		cin >> deathAgetest;

		}
		break;

	case 6: // run the model
		{

			repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();

			model->initAgents();
			model->initSchedule(runner);

			runner.run();

		}
		break;
	case 7: // exit
			delete model;

			repast::RepastProcess::instance()->done();
		cout << "End" << endl;
		break;
	}
}






}
