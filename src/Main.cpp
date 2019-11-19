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

while(mode!=8){
	cout << "Select an option:" << endl;
	cout << "1. Test 1" << endl;
	cout << "2. Test 2" << endl;
	cout << "3. Test 3" << endl;
	cout << "4. Test 4" << endl;
	cout << "5. Test 5" << endl;
	cout << "6. Test 6" << endl;
	cout << "7. Run Model" << endl;
	cout << "8. Exit" << endl;

	cin >> mode;

	switch(mode)
	{
	case 1:
		// run test1
		{
		repast::AgentId id(32, 1, 1);
		Household h1(id,1,2,3);
		//cout << h1.getHouseholdId() << endl;
		//cout << h1.getAssignedField() << endl;
		cout << h1.getMaizeStorage()  << endl;
		cout << h1.getAge()  << endl;
		cout << h1.getDeathAge()  << endl;
		}
		break;
	case 2: // run test2
		cout << "hello 2" << endl;
		break;
	case 3: // run test3
		cout << "hello 3" << endl;
		break;
	case 4: // run test4
		cout << "hello 4" << endl;
		break;
	case 5: // run test5 not wokring
		{
		cout << "set new death age" << endl;
		cin >> deathAgetest;
		// repast::Properties::putProperty("max.age",deathAgetest); not working
		}
		break;
	case 6: // run test6
		model->OutputFile();
		break;
	case 7: // run the model
		{

			std::string configFile = argv[1]; // The name of the configuration file is Arg 1
			std::string propsFile  = argv[2]; // The name of the properties file is Arg 2

			boost::mpi::environment env(argc, argv);
			boost::mpi::communicator world;

			repast::RepastProcess::init(configFile);


			AnasaziModel* model = new AnasaziModel(propsFile, argc, argv, &world);
			repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();

			model->initAgents();
			model->initSchedule(runner);

			runner.run();
			/*
			delete model;

			repast::RepastProcess::instance()->done();
			*/
		}
		break;
	case 8: // exit
			delete model;

			repast::RepastProcess::instance()->done();
		cout << "End" << endl;
		break;
	}
}






}
