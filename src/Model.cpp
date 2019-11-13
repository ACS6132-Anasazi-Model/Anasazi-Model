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
#include <fstream>

#include "Model.h"

AnasaziModel::AnasaziModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm){
	props = new repast::Properties(propsFile, argc, argv, comm);
	stopAt = repast::strToInt(props->getProperty("stop.at"));
	boardSizeX = repast::strToInt(props->getProperty("board.size.x"));
	boardSizeY = repast::strToInt(props->getProperty("board.size.y"));

	initializeRandom(*props, comm);
	repast::Point<double> origin(0,0);
	repast::Point<double> extent(boardSizeX, boardSizeY);
	repast::GridDimensions gd (origin, extent);

	int procX = repast::strToInt(props->getProperty("proc.per.x"));
	int procY = repast::strToInt(props->getProperty("proc.per.y"));
	int bufferSize = repast::strToInt(props->getProperty("grid.buffer"));

	std::vector<int> processDims;
	processDims.push_back(procX);
	processDims.push_back(procY);
	discreteSpace = new repast::SharedDiscreteSpace<Agent, repast::StrictBorders, repast::SimpleAdder<Agent> >("AgentDiscreteSpace",gd,processDims,bufferSize, comm);

	context.addProjection(discreteSpace);

	Parameters param;
	//Add parameter file reading into struct here.
	param.startYear = repast::strToInt(props->getProperty("start.year"));
	param.endYear = repast::strToInt(props->getProperty("end.year"));
	param.maxStorageYear = repast::strToInt(props->getProperty("max.store.year"));
	param.householdNeed = repast::strToInt(props->getProperty("household.need"));
	param.minFissionAge = repast::strToInt(props->getProperty("fission.age"));
	param.maxAge = repast::strToInt(props->getProperty("max.age"));
	param.mostLikelyDeathAge = repast::strToInt(props->getProperty("most.death.age"));
	param.maxDistance = repast::strToInt(props->getProperty("max.distance"));
	param.initMinCorn = repast::strToInt(props->getProperty("initial.min.corn"));
	param.initMaxCorn = repast::strToInt(props->getProperty("initial.max.corn"));

	param.annualVariance = repast::strToDouble(props->getProperty("annual.variance"));
	param.spatialVariance = repast::strToDouble(props->getProperty("spatial.variance"));
	param.fertilityProbability = repast::strToDouble(props->getProperty("fertility.prop"));
	param.harvestAdjustment = repast::strToDouble(props->getProperty("harvest.adj"));

	fissionGen = repast::Random::instance()->createUniDoubleGenerator(0,1);
	deathAgeGen = repast::Random::instance()->createTriangleGenerator(0,param.mostLikelyDeathAge,param.maxAge);
}

AnasaziModel::~AnasaziModel(){
	delete props;
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
	std::ifstream file ("map.csv");//define file object and open map.csv
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
