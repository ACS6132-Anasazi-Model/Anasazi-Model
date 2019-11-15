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
#include "repast_hpc/Schedule.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/GridComponents.h"
#include <string>
#include <fstream>
#include <stdlib.h>

#include "Model.h"

AnasaziModel::AnasaziModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm), locationcontext(comm){
	props = new repast::Properties(propsFile, argc, argv, comm);
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
	householdSpace = new repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >("AgentDiscreteSpace",gd,processDims,bufferSize, comm);
	locationSpace = new repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >("LocationDiscreteSpace",gd,processDims,bufferSize, comm);

	context.addProjection(householdSpace);
	locationcontext.addProjection(locationSpace);

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

	year = param.startYear;
	stopAt = param.endYear - param.startYear + 1;

	repast::DoubleUniformGenerator fissionGen = repast::Random::instance()->createUniDoubleGenerator(0.0,1.0);
	repast::TriangleGenerator deathAgeGen = repast::Random::instance()->createTriangleGenerator(0.0,double(param.mostLikelyDeathAge),double(param.maxAge));
}

AnasaziModel::~AnasaziModel(){
	delete props;
}

void AnasaziModel::initAgents(){
	std::cout << "Initializing Model\n";
	int rank = repast::RepastProcess::instance()->rank();

	int LocationID = 0;
	for(int i=0; i<boardSizeX; i++ )
	{
		for(int j=0; j<boardSizeY; j++)
		{
			repast::AgentId id(LocationID, rank, 1);
			Location* agent = new Location(id);
			locationcontext.addAgent(agent);
			locationSpace->moveTo(id, repast::Point<int>(i, j));
			LocationID++;
		}
	}

	readcsv1(locationSpace);
	// std::vector<Location*> locationList;
	// locationSpace->getObjectsAt(repast::Point<int>(50, 50), locationList);
	// cout<< locationList[0]->getId().id() << "\n";
}

void AnasaziModel::doPerTick(){
	//std::cout << "Year " << year << "\n";
	++year;
}

void AnasaziModel::initSchedule(repast::ScheduleRunner& runner){
	runner.scheduleEvent(1, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<AnasaziModel> (this, &AnasaziModel::doPerTick)));
	runner.scheduleStop(stopAt);
}

bool AnasaziModel::fissionHousehold(){};
bool AnasaziModel::removeHousehold(){};
int AnasaziModel::countHousehold(){};

void AnasaziModel::readcsv1(repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >* locationSpace)
{
	int x,y,z , mz;
	string zone, maizeZone, temp;

	std::ifstream file ("map.csv");//define file object and open map.csv
	file.ignore(500,'\n');//Ignore first line

	while(1)//read until end of file
	{
		getline(file,temp,',');
		if(!temp.empty())
		{
			getline(file,temp,',');
			x = repast::strToInt(temp); //Read until ',' and convert to int & store in x
			getline(file,temp,',');
			y = repast::strToInt(temp); //Read until ',' and convert to int & store in y
			getline(file,temp,','); //skip data
			getline(file,zone,',');// read until ',' and store into zone
			getline(file,maizeZone,'\n');// read until next line and store into maizeZone
			std::cout << zone << std::endl;
			if(zone == "Empty")
			{
				z = 0;
			}
			else if(zone == "Natural")
			{
				z = 1;
			}
			else if(zone == "Kinbiko")
			{
				z = 2;
			}
			else if(zone == "Uplands")
			{
				z = 3;
			}
			else if(zone == "North")
			{
				z = 4;
			}
			else if(zone == "General")
			{
				z = 5;
			}
			else if(zone == "North Dunes")
			{
				z = 6;
			}
			else if(zone == "Mid Dunes")
			{
				z = 7;
			}
			else if(zone == "Mid")
			{
				z = 8;
			}
			else
			{
				z = 99;
				std::cout << zone << std::endl;
			}

			if(maizeZone == "Empty")
			{
				mz = 0;
			}
			else if(maizeZone == "No_Yield")
			{
				mz = 1;
			}
			else if(maizeZone == "Yield_1")
			{
				mz = 2;
			}
			else if(maizeZone == "Yield_2")
			{
				mz = 3;
			}
			else if(maizeZone == "Yield_3")
			{
				mz = 4;
			}
			else if(maizeZone == "Sand_dune")
			{
				mz = 5;
			}
			else
			{
				mz = 99;
				std::cout << maizeZone << std::endl;
			}
			std::vector<Location*> locationList;
			locationSpace->getObjectsAt(repast::Point<int>(x, y), locationList);
			locationList[0]->setZones(z,mz);
			//cout<< locationList[0]->getId().id() << "\n";
		}
		else{
			goto endloop;
		}
	}
	endloop: ;
}

void AnasaziModel::readcsv2(repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >* locationSpace)
{
	//read "start date","end date","median date","baseline households","x","y"
	int *startdate, *enddate, *mediandate, *baselinehouseholds, *x, *y;
	string temp;
	int i = 0, NoOfLine = 0;

	std::ifstream file ("settlement.csv");//define file object and open settlement.csv
	file.ignore(500,'\n');//Ignore first line
	while(!file.eof())
	{
		getline(file,temp);
		++NoOfLine;
	}

	startdate = (int*)malloc(NoOfLine*sizeof(int));
	enddate = (int*)malloc(NoOfLine*sizeof(int));
	mediandate = (int*)malloc(NoOfLine*sizeof(int));
	baselinehouseholds = (int*)malloc(NoOfLine*sizeof(int));
	x = (int*)malloc(NoOfLine*sizeof(int));
	y = (int*)malloc(NoOfLine*sizeof(int));


	file.clear();  // Go back to start
	file.seekg( 0 );
	while(!file.eof())//read until end of file
	{
		getline(file,temp,',');
		getline(file,temp,',');
		getline(file,temp,',');
		getline(file,temp,',');
		startdate[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		enddate[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		mediandate[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');  //skip data
		getline(file,temp,',');
		getline(file,temp,',');
		getline(file,temp,',');
		getline(file,temp,',');
		getline(file,temp,',');
		baselinehouseholds[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		x[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		y[i] = repast::strToInt(temp); //Read until ',' and convert to int
		i++;
	}
}

void AnasaziModel::readcsv3(repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >* locationSpace)
{
	//read "id number","meters north","meters east","type","start date","end date","x","y"
	int *type, *startdate, *enddate, *x, *y;
	string temp;
	int i = 0, NoOfLine = 0;

	std::ifstream file ("water.csv");//define file object and open water.csv
	file.ignore(500,'\n');//Ignore first line
	while(!file.eof())
	{
		getline(file,temp);
		++NoOfLine;
	}

	type = (int*)malloc(NoOfLine*sizeof(int));
	startdate = (int*)malloc(NoOfLine*sizeof(int));
	enddate = (int*)malloc(NoOfLine*sizeof(int));
	x = (int*)malloc(NoOfLine*sizeof(int));
	y = (int*)malloc(NoOfLine*sizeof(int));


	file.clear();  // Go back to start
	file.seekg( 0 );
	while(!file.eof())//read until end of file
	{
		getline(file,temp,',');
		getline(file,temp,',');
		getline(file,temp,',');
		getline(file,temp,',');
		type[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		startdate[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		enddate[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		x[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		y[i] = repast::strToInt(temp); //Read until ',' and convert to int
		i++;
	}
}

void AnasaziModel::readcsv4(repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >* locationSpace)
{
	//read "year","general","north","mid","natural","upland","kinbiko"
	int *pdsiyear;
	double *pdsigeneral, *pdsinorth, *pdsimid, *pdsinatural, *pdsiupland, *pdsikinbiko;
	string temp;
	int i = 0, NoOfLine = 0;

	std::ifstream file ("pdsi.csv");//define file object and open pdsi.csv
	file.ignore(500,'\n');//Ignore first line
	while(!file.eof())
	{
		getline(file,temp);
		++NoOfLine;
	}

	pdsiyear = (int*)malloc(NoOfLine*sizeof(int));
	pdsigeneral = (double*)malloc(NoOfLine*sizeof(double));
	pdsinorth = (double*)malloc(NoOfLine*sizeof(double));
	pdsimid = (double*)malloc(NoOfLine*sizeof(double));
	pdsinatural = (double*)malloc(NoOfLine*sizeof(double));
	pdsiupland = (double*)malloc(NoOfLine*sizeof(double));
	pdsikinbiko = (double*)malloc(NoOfLine*sizeof(double));

	file.clear();  // Go back to start
	file.seekg( 0 );
	while(!file.eof())//read until end of file
	{
		getline(file,temp,',');
		pdsiyear[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		pdsigeneral[i] = repast::strToDouble(temp); //Read until ',' and convert to double
		getline(file,temp,',');
		pdsinorth[i] = repast::strToDouble(temp); //Read until ',' and convert to double
		getline(file,temp,',');
		pdsimid[i] = repast::strToDouble(temp); //Read until ',' and convert to double
		getline(file,temp,',');
		pdsinatural[i] = repast::strToDouble(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		pdsiupland[i] = repast::strToDouble(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		pdsikinbiko[i] = repast::strToDouble(temp); //Read until ',' and convert to double
		i++;
	}
}

void AnasaziModel::readcsv5(repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >* locationSpace)
{
	//read "year","general","north","mid","natural","upland","kinbiko"
	int *hydroyear;
	double *hydrogeneral, *hydronorth, *hydromid, *hydronatural, *hydroupland, *hydrokinbiko;
	string temp;
	int i = 0, NoOfLine = 0;

	std::ifstream file ("hydro.csv");//define file object and open hydro.csv
	file.ignore(500,'\n');//Ignore first line
	while(!file.eof())
	{
		getline(file,temp);
		++NoOfLine;
	}

	hydroyear = (int*)malloc(NoOfLine*sizeof(int));
	hydrogeneral = (double*)malloc(NoOfLine*sizeof(double));
	hydronorth = (double*)malloc(NoOfLine*sizeof(double));
	hydromid = (double*)malloc(NoOfLine*sizeof(double));
	hydronatural = (double*)malloc(NoOfLine*sizeof(double));
	hydroupland = (double*)malloc(NoOfLine*sizeof(double));
	hydrokinbiko = (double*)malloc(NoOfLine*sizeof(double));

	file.clear();  // Go back to start
	file.seekg( 0 );
	while(!file.eof())//read until end of file
	{
		getline(file,temp,',');
		hydroyear[i] = repast::strToInt(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		hydrogeneral[i] = repast::strToDouble(temp); //Read until ',' and convert to double
		getline(file,temp,',');
		hydronorth[i] = repast::strToDouble(temp); //Read until ',' and convert to double
		getline(file,temp,',');
		hydromid[i] = repast::strToDouble(temp); //Read until ',' and convert to double
		getline(file,temp,',');
		hydronatural[i] = repast::strToDouble(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		hydroupland[i] = repast::strToDouble(temp); //Read until ',' and convert to int
		getline(file,temp,',');
		hydrokinbiko[i] = repast::strToDouble(temp); //Read until ',' and convert to double
		i++;
	}
}
