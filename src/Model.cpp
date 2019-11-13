#include <stdio.h>
#include <vector>
#include <boost/mpi.hpp>
#include "repast_hpc/AgentId.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Utilities.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/initialize_random.h"
#include "repast_hpc/SVDataSetBuilder.h"
#include "repast_hpc/Point.h"
#include "repast_hpc/Random.h"
#include <string>

#include "Model.h"

AnasaziModel::AnasaziModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm){

}

AnasaziModel::~AnasaziModel(){

}

void AnasaziModel::initAgents(){

}

void AnasaziModel::doPerTick(){

}

void AnasaziModel::initSchedule(repast::ScheduleRunner& runner){
	runner.scheduleEvent(1, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<SchellingModel> (this, &SchellingModel::doPerTick)));
	runner.scheduleStop(stopAt);
}

bool AnasaziModel::fissionHousehold();
bool AnasaziModel::removeHousehold();
int AnasaziModel::countHousehold();
void AnasaziModel::readcsv1()
{
	int x[], y[];
	string zone[], maizeZone[];
	char temp[];
	int i = 0;
	std::ifstream file;//define file object
	file.open("map.csv", ios::in); //Open file map.csv
	file.ignore(500,'\n')//Ignore first line
	while(!file.eof())//read until end of file
	{
		getline(file,temp,',');
		x[i] = repast::strToInt(temp); //Read until ',' and convert to int & store in x
		getline(file,temp,',');
		y[i] = repast::strToInt(temp); //Read until ',' and convert to int & store in y
		getline(file,temp,','); //skip data
		getline(file,zone[i],',');// read until ',' and store into zone
		getline(file,maizeZone[i],'\n');// read until next line and store into maizeZone
		i++;
	}
}
