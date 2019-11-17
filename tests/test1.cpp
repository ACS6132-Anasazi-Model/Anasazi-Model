#include <stdio.h>
#include <vector>
#include "repast_hpc/RepastProcess.h"
#include <boost/mpi.hpp>

#include "Model.h"
#include "Location.h"
#include "Household.h"



/*
int main(){

AnasaziModel model //need to declare the class I'm using then call out each of the functions inside.

//model.initAgents();
cout << "No Households" << endl;



}
*/



int main(int argc, char** argv){

  std::string configFile = argv[1]; // The name of the configuration file is Arg 1
  std::string propsFile  = argv[2]; // The name of the properties file is Arg 2

  boost::mpi::environment env(argc, argv);
  boost::mpi::communicator world;

  repast::RepastProcess::init(configFile);

  AnasaziModel* model = new AnasaziModel(propsFile, argc, argv, &world);
  repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner();

  model->initAgents();
  //model->initSchedule(runner);

  //runner.run();

	//delete model;

	//repast::RepastProcess::instance()->done();

  }
