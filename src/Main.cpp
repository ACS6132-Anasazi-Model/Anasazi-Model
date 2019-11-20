#include <boost/mpi.hpp>
#include "repast_hpc/RepastProcess.h"

#include "Model.h"
#include "Location.h"
#include "Household.h"
#include <iomanip>


int main(int argc, char** argv){


	std::string configFile = argv[1]; // The name of the configuration file is Arg 1
	std::string propsFile  = argv[2]; // The name of the properties file is Arg 2

	boost::mpi::environment env(argc, argv);
	boost::mpi::communicator* world;


	/*
	repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();

	model->initAgents();
	model->initSchedule(runner);

	runner.run();

	delete model;

	repast::RepastProcess::instance()->done();
	*/


	int mode; //	select whether to run test 1-6, run the model or exit
	int maizeYI; // set maize yield index
	int deathAgetest; // set input age value
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

		int b;
		int c;
		int d;
		double soil = 0;
		int yield;
		double harvestAdjustment;
		double randomNum =0;

		cout << "Pick agent:" << endl;
		cin >> pseudoID;
		repast::AgentId id(pseudoID, 0, 0);
		Household* h1;
		cout << "Soil quality and randomNum set to 0:"<<endl;
		cout << "Enter Age:"<<endl;
		cin >> b;
		cout << "Enter Death Age:"<<endl;
		cin >> c;
		cout << "Enter Maize Storage:"<<endl;
		cin >> d;
		h1 = new Household(id,b,c,d);
		cout << "Enter maize yield for field:"<<endl;
		cin >> yield;
		cout << "Enter Harvest Adjustment (between 0 and 1):"<<endl;
		cin >> harvestAdjustment;
		int totalMaize = yield * harvestAdjustment + h1->getMaizeStorage() -800;
		cout << "Expected Maize storage:" << totalMaize << endl;

		cout << h1 -> getId() << endl;
		cout << "Age:" << h1->getAge()  << endl;
		cout << "Death Age:" << h1->getDeathAge()  << endl;
		cout << "Maize Storage:" << h1->getMaizeStorage()  << endl;
		//cout << "Assigned Field:" << h1.getAssignedField() << endl;
		Location L1(id, soil);
		L1.calculateYield(yield,harvestAdjustment,randomNum);
		h1 ->chooseField( &L1 );
		cout << "Moving to next year" << endl;
		h1 ->nextYear();
		cout << "Maize Storage:" << h1->getMaizeStorage()  << endl;

		if(h1->getMaizeStorage()==totalMaize)
		{
			cout << "\033[1;32mTest Passed\033[0m\n";
		}
		else
		{
			cout << "\033[1;31mTest Failed\033[0m\n";
		}


		// run something


		//print out again

		}
		break;
	case 2:  // run test2
		{
			repast::RepastProcess::init(configFile);
			world = new boost::mpi::communicator;
			AnasaziModel* model = new AnasaziModel(propsFile, argc, argv, world);
			model->test2(world);
			delete model;
			repast::RepastProcess::instance()->done();
		}
		break;
	case 3: // run test3
		// compare the archaelogical data from water csv

		{
			repast::RepastProcess::init(configFile);
			world = new boost::mpi::communicator;
			AnasaziModel* model = new AnasaziModel(propsFile, argc, argv, world);
			model->initAgents();
			model->test3();
			delete model;
			repast::RepastProcess::instance()->done();
		}

		break;
	case 4: // run test4
	{
		double harvestAdjustment; // set input age value
		cout << "set new yield adjustment" << endl;
		cin >> harvestAdjustment;

		fstream file;
		file.open("props/model.props", ios::in | ios::out);
		string temp;
		int pos= file.tellg();
		getline(file,temp);
		while(temp.find("harvest.adj") == std::string::npos)
		{
			pos= file.tellg();
			getline(file,temp);
		}
		file.seekp(pos);
		file.seekg(pos);
		file << "harvest.adj=" << std::setw(5) << harvestAdjustment;

		file.close();
		goto RunModel;
		break;
	}
	case 5: // run test5 not wokring
		{
		int deathAgetest; // set input age value
		cout << "set new death age" << endl;
		cin >> deathAgetest;

		fstream file;
    file.open("props/model.props", ios::in | ios::out);
		string temp;
		int pos= file.tellg();
		getline(file,temp);
    while(temp.find("max.age") == std::string::npos)
		{
			pos= file.tellg();
			getline(file,temp);
    }
		file.seekp(pos);
		file.seekg(pos);
		file << std::setw(5);
		file << "max.age=" << std::setw(5) << deathAgetest;

    file.close();
		goto RunModel;
		// repast::Properties::putProperty("max.age",deathAgetest); not working
		}
		break;

	case 6: // run the model
		{
			RunModel:
			repast::RepastProcess::init(configFile);
			world = new boost::mpi::communicator;
			AnasaziModel* model = new AnasaziModel(propsFile, argc, argv, world);
			repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();

			model->initAgents();
			model->initSchedule(runner);

			runner.run();
			delete model;
			repast::RepastProcess::instance()->done();

		}
		break;
	case 7: // exit

		cout << "End" << endl;
		break;
	}
}
}
