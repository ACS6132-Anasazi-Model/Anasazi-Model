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
#include "repast_hpc/Moore2DGridQuery.h"

#include "Model.h"
repast::DoubleUniformGenerator* fissionGen;
extern repast::TriangleGenerator deathAgeGen;
extern repast::NormalGenerator yieldGen;
extern repast::NormalGenerator soilGen;
extern repast::IntUniformGenerator initAgeGen;
extern repast::IntUniformGenerator initMaizeGen;

AnasaziModel::AnasaziModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm) , locationcontext(comm)
{
	props = new repast::Properties(propsFile, argc, argv, comm);
	boardSizeX = repast::strToInt(props->getProperty("board.size.x"));
	boardSizeY = repast::strToInt(props->getProperty("board.size.y"));
	//randomSeed = repast::strToInt(props->getProperty("random.seed"));

	//repast::Random::instance()->initialize(randomSeed);
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
	param.maxStorage = repast::strToInt(props->getProperty("max.storage"));
	param.householdNeed = repast::strToInt(props->getProperty("household.need"));
	param.minFissionAge = repast::strToInt(props->getProperty("min.fission.age"));
	param.maxFissionAge = repast::strToInt(props->getProperty("max.fission.age"));
	param.maxAge = repast::strToInt(props->getProperty("max.age"));
	param.maxDistance = repast::strToInt(props->getProperty("max.distance"));
	param.initMinCorn = repast::strToInt(props->getProperty("initial.min.corn"));
	param.initMaxCorn = repast::strToInt(props->getProperty("initial.max.corn"));

	param.annualVariance = repast::strToDouble(props->getProperty("annual.variance"));
	param.spatialVariance = repast::strToDouble(props->getProperty("spatial.variance"));
	param.fertilityProbability = repast::strToDouble(props->getProperty("fertility.prop"));
	param.harvestAdjustment = repast::strToDouble(props->getProperty("harvest.adj"));
	param.maizeStorageRatio = repast::strToDouble(props->getProperty("new.household.ini.maize"));

	year = param.startYear;
	stopAt = param.endYear - param.startYear + 1;
	std::string String1 = "fissionGen";
	fissionGen = new repast::DoubleUniformGenerator(repast::Random::instance()->createUniDoubleGenerator(0,1));
	//repast::DoubleUniformGenerator fissionGen = fissionGenDup;
	deathAgeGen = new repast::TriangleGenerator(repast::Random::instance()->createTriangleGenerator(0,param.maxAge+1,param.maxAge+1));
	yieldGen = new repast::NormalGenerator(repast::Random::instance()->createNormalGenerator(0,param.annualVariance));
	soilGen = new repast::NormalGenerator(repast::Random::instance()->createNormalGenerator(0,param.spatialVariance));
	initAgeGen = new repast::IntUniformGenerator(repast::Random::instance()->createUniIntGenerator(0,29));
	initMaizeGen = new repast::IntUniformGenerator(repast::Random::instance()->createUniIntGenerator(param.initMinCorn,param.initMaxCorn));

	out.open("NumberOfHousehold.csv");
	out << "Year, Number of Households" << endl;
}

AnasaziModel::~AnasaziModel()
{
	delete props;
	out.close();
}

void AnasaziModel::initAgents()
{

	std::cout << "Initializing Model\n";
	int rank = repast::RepastProcess::instance()->rank();

	int LocationID = 0;
	for(int i=0; i<boardSizeX; i++ )
	{
		for(int j=0; j<boardSizeY; j++)
		{
			repast::AgentId id(LocationID, rank, 1);
			Location* agent = new Location(id, soilGen->next());
			locationcontext.addAgent(agent);
			locationSpace->moveTo(id, repast::Point<int>(i, j));
			LocationID++;
		}
	}

	readcsv1();
	readcsv2();
	readcsv3();
	readcsv4();
	readcsv5();
	//updateHouseholdProperties();
	updateLocationProperties();

	repast::SharedContext<Household>::const_iterator local_agents_iter = context.begin();
	repast::SharedContext<Household>::const_iterator local_agents_end = context.end();

	while(local_agents_iter != local_agents_end)
	{
		Household* household = (&**local_agents_iter);
		if(household->death())
		{
			repast::AgentId id = household->getId();
			local_agents_iter++;

			std::vector<int> loc;
			householdSpace->getLocation(id, loc);

			std::vector<Location*> locationList;
			if(!loc.empty())
			{
				locationSpace->getObjectsAt(repast::Point<int>(loc[0], loc[1]), locationList);
				locationList[0]->setState(0);
			}
			cout << "\033[1;31mAgent " << household->getId().id() << " is already dead.\033[0m\n";
			context.removeAgent(id);
			//repast::RepastProcess::instance()->agentRemoved(id);
		}
		else
		{
			local_agents_iter++;
			fieldSearch(household);
		}
	}
}

void AnasaziModel::doPerTick()
{
	updateLocationProperties();
	OutputFile();
	#ifdef DEBUG
		std::cout << "\033[4;7m" << "Year " << year << "\033[0m\n";
	#endif
	year++;
	updateHouseholdProperties();
}

void AnasaziModel::initSchedule(repast::ScheduleRunner& runner)
{
	runner.scheduleEvent(1, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<AnasaziModel> (this, &AnasaziModel::doPerTick)));
	runner.scheduleStop(stopAt);
}

void AnasaziModel::readcsv1()
{
	int x,y,z , mz;
	string zone, maizeZone, temp;

	std::ifstream file ("data/map.csv");//define file object and open map.csv
	file.ignore(500,'\n');//Ignore first line

	while(1)//read until end of file
	{
		getline(file,temp,',');
		if(!temp.empty())
		{
			//getline(file,temp,',');
			x = repast::strToInt(temp); //Read until ',' and convert to int & store in x
			getline(file,temp,',');
			y = repast::strToInt(temp); //Read until ',' and convert to int & store in y
			getline(file,temp,','); //colour
			getline(file,zone,',');// read until ',' and store into zone
			getline(file,maizeZone,'\n');// read until next line and store into maizeZone
			//cout << zone << "," << maizeZone << endl;
			if(zone == "\"Empty\"")
			{
				z = 0;
			}
			else if(zone == "\"Natural\"")
			{
				z = 1;
			}
			else if(zone == "\"Kinbiko\"")
			{
				z = 2;
			}
			else if(zone == "\"Uplands\"")
			{
				z = 3;
			}
			else if(zone == "\"North\"")
			{
				z = 4;
			}
			else if(zone == "\"General\"")
			{
				z = 5;
			}
			else if(zone == "\"North Dunes\"")
			{
				z = 6;
			}
			else if(zone == "\"Mid Dunes\"")
			{
				z = 7;
			}
			else if(zone == "\"Mid\"")
			{
				z = 8;
			}
			else
			{
				z = 99;
				std::cout << zone << std::endl;
			}

			if(maizeZone.find("Empty") != std::string::npos)
			{
				mz = 0;
			}
			else if(maizeZone.find("No_Yield") != std::string::npos)
			{
				mz = 1;
			}
			else if(maizeZone.find("Yield_1") != std::string::npos)
			{
				mz = 2;
			}
			else if(maizeZone.find("Yield_2") != std::string::npos)
			{
				mz = 3;
			}
			else if(maizeZone.find("Yield_3") != std::string::npos)
			{
				mz = 4;
			}
			else if(maizeZone.find("Sand_dune") != std::string::npos)
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

void AnasaziModel::readcsv2()
{

	//read "start date","end date","median date","baseline households","x","y"
	int startdate, enddate, baselinehouseholds, x, y;
	string temp;
	int rank = repast::RepastProcess::instance()->rank();
	std::ifstream file ("data/settlement.csv");//define file object and open settlement.csv
	file.ignore(500,'\n');//Ignore first line
	while(1)//read until end of file
	{
		getline(file,temp,',');
		if(!temp.empty())
		{
			getline(file,temp,',');
			getline(file,temp,',');
			getline(file,temp,',');
			startdate = repast::strToInt(temp); //Read until ',' and convert to int
			getline(file,temp,',');
			enddate = repast::strToInt(temp); //Read until ',' and convert to int
			getline(file,temp,',');
			getline(file,temp,',');  //skip data
			getline(file,temp,',');
			getline(file,temp,',');
			getline(file,temp,',');
			getline(file,temp,',');
			getline(file,temp,',');
			baselinehouseholds = repast::strToInt(temp); //Read until ',' and convert to int
			getline(file,temp,',');
			x = repast::strToInt(temp); //Read until ',' and convert to int
			getline(file,temp,'\n');
			y = repast::strToInt(temp); //Read until ',' and convert to int
			if(startdate<=800 && enddate >800)
			{
				repast::AgentId id(houseID, rank, 2);
				int initAge = initAgeGen->next();
				int mStorage = initMaizeGen->next();
				Household* agent = new Household(id, initAge, deathAgeGen->next(), mStorage);
				context.addAgent(agent);
				householdSpace->moveTo(id, repast::Point<int>(x, y));

				std::vector<Location*> locationList;
				locationSpace->getObjectsAt(repast::Point<int>(x, y), locationList);
				locationList[0]->setState(1);

				houseID++;
				#ifdef DEBUG
					std::cout << id.id() << ", x= " << x << ", y= " << y << ", Age= " << initAge << ", mStorage=" << mStorage << std::endl;
				#endif
			}
		}
		else
		{
			goto endloop;
		}
	}
	endloop: ;
}

void AnasaziModel::readcsv3()
{
	//read "type","start date","end date","x","y"
	int type, startYear, endYear, x, y;
	string temp;

	std::ifstream file ("data/water.csv");//define file object and open water.csv
	file.ignore(500,'\n');//Ignore first line
	while(1)//read until end of file
	{
		getline(file,temp,',');
		if(!temp.empty())
		{
			getline(file,temp,',');
			getline(file,temp,',');
			getline(file,temp,',');
			type = repast::strToInt(temp); //Read until ',' and convert to int
			getline(file,temp,',');
			startYear = repast::strToInt(temp); //Read until ',' and convert to int
			getline(file,temp,',');
			endYear = repast::strToInt(temp); //Read until ',' and convert to int
			getline(file,temp,',');
			x = repast::strToInt(temp); //Read until ',' and convert to int
			getline(file,temp,'\n');
			y = repast::strToInt(temp); //Read until ',' and convert to int

			std::vector<Location*> locationList;
			locationSpace->getObjectsAt(repast::Point<int>(x, y), locationList);
			locationList[0]->addWaterSource(type,startYear, endYear);
			//locationList[0]->checkWater(existStreams, existAlluvium, x, y, year);
		}
		else
		{
			goto endloop;
		}
	}
	endloop: ;
}

void AnasaziModel::readcsv4()
{
	//read "year","general","north","mid","natural","upland","kinbiko"
	int i=0;
	string temp;

	std::ifstream file ("data/pdsi.csv");//define file object and open pdsi.csv
	file.ignore(500,'\n');//Ignore first line

	while(1)//read until end of file
	{
		getline(file,temp,',');
		if(!temp.empty())
		{
			pdsi[i].year = repast::strToInt(temp); //Read until ',' and convert to int
			getline(file,temp,',');
			pdsi[i].pdsiGeneral = repast::strToDouble(temp); //Read until ',' and convert to double
			getline(file,temp,',');
			pdsi[i].pdsiNorth = repast::strToDouble(temp); //Read until ',' and convert to double
			getline(file,temp,',');
			pdsi[i].pdsiMid = repast::strToDouble(temp); //Read until ',' and convert to double
			getline(file,temp,',');
			pdsi[i].pdsiNatural = repast::strToDouble(temp); //Read until ',' and convert to int
			getline(file,temp,',');
			pdsi[i].pdsiUpland = repast::strToDouble(temp); //Read until ',' and convert to int
			getline(file,temp,'\n');
			pdsi[i].pdsiKinbiko = repast::strToDouble(temp); //Read until ',' and convert to double
			i++;
		}
		else{
			goto endloop;
		}
	}
	endloop: ;
}

void AnasaziModel::readcsv5()
{
	//read "year","general","north","mid","natural","upland","kinbiko"
	string temp;
	int i =0;

	std::ifstream file ("data/hydro.csv");//define file object and open hydro.csv
	file.ignore(500,'\n');//Ignore first line

	while(1)//read until end of file
	{
		getline(file,temp,',');
		if(!temp.empty())
		{
			hydro[i].year = repast::strToInt(temp); //Read until ',' and convert to int
			getline(file,temp,',');
			hydro[i].hydroGeneral = repast::strToDouble(temp); //Read until ',' and convert to double
			getline(file,temp,',');
			hydro[i].hydroNorth = repast::strToDouble(temp); //Read until ',' and convert to double
			getline(file,temp,',');
			hydro[i].hydroMid = repast::strToDouble(temp); //Read until ',' and convert to double
			getline(file,temp,',');
			hydro[i].hydroNatural = repast::strToDouble(temp); //Read until ',' and convert to int
			getline(file,temp,',');
			hydro[i].hydroUpland = repast::strToDouble(temp); //Read until ',' and convert to int
			getline(file,temp,'\n');
			hydro[i].hydroKinbiko = repast::strToDouble(temp); //Read until ',' and convert to double
			i++;
		}
		else
		{
			goto endloop;
		}
	}
	endloop: ;
}

int AnasaziModel::yieldFromPDSI(int zone, int maizeZone)
{
	int pdsiValue, row, col;
	switch(zone)
	{
		case 1:
			pdsiValue = pdsi[year-param.startYear].pdsiNatural;
			break;
		case 2:
			pdsiValue = pdsi[year-param.startYear].pdsiKinbiko;
			break;
		case 3:
			pdsiValue = pdsi[year-param.startYear].pdsiUpland;
			break;
		case 4:
		case 6:
			pdsiValue = pdsi[year-param.startYear].pdsiNorth;
			break;
		case 5:
			pdsiValue = pdsi[year-param.startYear].pdsiGeneral;
			break;
		case 7:
		case 8:
			pdsiValue = pdsi[year-param.startYear].pdsiMid;
			break;
		default:
			return 0;
	}

	/* Rows of pdsi table*/
	if(pdsiValue < -3)
	{
		row = 0;
	}
	else if(pdsiValue >= -3 && pdsiValue < -1)
	{
		row = 1;
	}
	else if(pdsiValue >= -1 && pdsiValue < 1)
	{
		row = 2;
	}
	else if(pdsiValue >= 1 && pdsiValue < 3)
	{
		row = 3;
	}
	else if(pdsiValue >= 3)
	{
		row = 4;
	}
	else
	{
		return 0;
	}

	/* Col of pdsi table*/
	if(maizeZone >= 2)
	{
		col = maizeZone - 2;
	}
	else
	{
		return 0;
	}

	return yieldLevels[row][col];
}

double AnasaziModel::hydroLevel(int zone)
{
	switch(zone)
	{
		case 1:
			return hydro[year-param.startYear].hydroNatural;
		case 2:
			return hydro[year-param.startYear].hydroKinbiko;
		case 3:
			return hydro[year-param.startYear].hydroUpland;
		case 4:
		case 6:
			return hydro[year-param.startYear].hydroNorth;
		case 5:
			return hydro[year-param.startYear].hydroGeneral;
		case 7:
		case 8:
			return hydro[year-param.startYear].hydroMid;
		default:
			return 0;
	}
}

void AnasaziModel::checkWaterConditions()
{
	if ((year >= 280 && year < 360) or (year >= 800 && year < 930) or (year >= 1300 && year < 1450))
	{
		existStreams = true;
	}
	else
	{
		existStreams = false;
	}

	if (((year >= 420) && (year < 560)) or ((year >= 630) && (year < 680)) or	((year >= 980) && (year < 1120)) or ((year >= 1180) && (year < 1230)))
	{
		existAlluvium = true;
	}
	else
	{
		existAlluvium = false;
	}
}

void AnasaziModel::OutputFile()
{
	out << year << ", " <<  context.size() << std::endl;
}

void  AnasaziModel::updateLocationProperties()
{
	checkWaterConditions();
	int x = 0;
	for(int i=0; i<boardSizeX; i++ )
	{
		for(int j=0; j<boardSizeY; j++)
		{
			std::vector<Location*> locationList;
			locationSpace->getObjectsAt(repast::Point<int>(i, j), locationList);
			locationList[0]->checkWater(existStreams,existAlluvium, i, j, year);
			int mz = locationList[0]->getMaizeZone();
			int z = locationList[0]->getZone();
			int y = yieldFromPDSI(z,mz);
			locationList[0]->calculateYield(y, param.harvestAdjustment, yieldGen->next());
		}
	}
}

void AnasaziModel::updateHouseholdProperties()
{
	repast::SharedContext<Household>::const_iterator local_agents_iter = context.begin();
	repast::SharedContext<Household>::const_iterator local_agents_end = context.end();

	while(local_agents_iter != local_agents_end)
	{
		Household* household = (&**local_agents_iter);
		if(household->death())
		{
			local_agents_iter++;
			cout << "\033[1;31mAgent " << household->getId().id() << " has died peacefully.\033[0m\n";
			removeHousehold(household);

		}
		else
		{
			local_agents_iter++;
			if(household->fission(param.minFissionAge,param.maxFissionAge, fissionGen->next()))
			{
				int rank = repast::RepastProcess::instance()->rank();
				repast::AgentId id(houseID, rank, 2);
				int mStorage = household->splitMaizeStored(param.maizeStorageRatio);
				Household* newAgent = new Household(id, 0, deathAgeGen->next(), mStorage);
				context.addAgent(newAgent);

				std::vector<int> loc;
				householdSpace->getLocation(household->getId(), loc);
				householdSpace->moveTo(id, repast::Point<int>(loc[0], loc[1]));
				cout << "\033[1;33mA new household " << id.id() << " is born\033[0m\n";
				fieldSearch(newAgent);
				houseID++;
			}

			bool fieldFound = true;
			if(!(household->checkMaize()))
			{
				cout << "\033[1;34mI need a new field\033[0m" << endl;
				fieldFound = fieldSearch(household);
			}
			if(fieldFound)
			{
				household->nextYear();
				cout << "Expected Yield = " << household->getAssignedField()->getExpectedYield() << endl;
			}
		}
	}
}

bool AnasaziModel::fieldSearch(Household* household)
{
	/******** Choose Field ********/
	std::vector<int> loc;
	householdSpace->getLocation(household->getId(), loc);
	repast::Point<int> center(loc);

	std::vector<Location*> neighbouringLocations;
	//std::vector<Location*> checkedLocations;
	repast::Moore2DGridQuery<Location> moore2DQuery(locationSpace);
	int range = 1;
	while(1)
	{
		moore2DQuery.query(loc, range, false, neighbouringLocations);
		// for(std::vector<Location>::iterator it = checkedLocations.begin() ; it != checkedLocations.end(); ++it)
		// {
		// 	neighbouringLocations.erase(std::remove(neighbouringLocations.begin(), neighbouringLocations.end(),(&*it)),neighbouringLocations.end());
		// }

		for (std::vector<Location*>::iterator it = neighbouringLocations.begin() ; it != neighbouringLocations.end(); ++it)
		{
			Location* tempLoc = (&**it);
			//cout << "Yield = " << tempLoc->getExpectedYield() << endl;
			if(tempLoc->getState() == 0)
			{
				if(tempLoc->getExpectedYield() >= 800)
				{
					std::vector<int> loc;
					locationSpace->getLocation(tempLoc->getId(), loc);
					cout << "\033[1;32mFound a field (" << loc[0] << "," << loc[1] << "), yield = " << tempLoc->getExpectedYield() << "\033[0m"<< endl;
					tempLoc->setState(2);
					household->chooseField(tempLoc);
					goto EndOfLoop;
				}
			}
		}
		range++;
		if(range > boardSizeY)
		{
			cout << "\033[1;31mAgent " << household->getId().id() << " left valley. No suitable field\033[0m\n";
			removeHousehold(household);
			return false;
		}
	}
	EndOfLoop:
	if(range >= 10)
	{
		return relocateHousehold(household);
	}
	else
	{
		return true;
	}
}

void AnasaziModel::removeHousehold(Household* household)
{
	repast::AgentId id = household->getId();

	std::vector<int> loc;
	householdSpace->getLocation(id, loc);

	std::vector<Location*> locationList;
	std::vector<Household*> householdList;
	if(!loc.empty())
	{
		locationSpace->getObjectsAt(repast::Point<int>(loc[0], loc[1]), locationList);
		householdSpace->getObjectsAt(repast::Point<int>(loc[0], loc[1]), householdList);
		if(householdList.size() == 1)
		{
			//cout << locationList.size() << endl;
			locationList[0]->setState(0);
		}
		if(household->getAssignedField()!= NULL)
		{
			std::vector<int> loc;
			locationSpace->getLocation(household->getAssignedField()->getId(), loc);
			locationSpace->getObjectsAt(repast::Point<int>(loc[0], loc[1]), locationList);
			locationList[0]->setState(0);
		}
	}

	context.removeAgent(id);
	//repast::RepastProcess::instance()->agentRemoved(id);
}

bool AnasaziModel::relocateHousehold(Household* household)
{
	cout << "\033[1;34mI need to relocate\033[0m\n";

	std::vector<Location*> neighbouringLocations;
	std::vector<Location*> suitableLocations;
	std::vector<Location*> waterSources;
	//std::vector<Location*> checkedLocations;

	std::vector<int> loc, loc2;
	locationSpace->getLocation(household->getAssignedField()->getId(), loc);
	householdSpace->getLocation(household->getId(),loc2);

	locationSpace->getObjectsAt(repast::Point<int>(loc2[0], loc2[1]), neighbouringLocations);
	Location* householdLocation = neighbouringLocations[0];

	repast::Point<int> center(loc);
	repast::Moore2DGridQuery<Location> moore2DQuery(locationSpace);
	int range = floor(param.maxDistance/100);
	int i = 1;
	bool conditionC = true;

	//get all !Field with 1km
	LocationSearch:
		moore2DQuery.query(loc, range*i, false, neighbouringLocations);
		for (std::vector<Location*>::iterator it = neighbouringLocations.begin() ; it != neighbouringLocations.end(); ++it)
		{
			Location* tempLoc = (&**it);
			//cout << "Yield = " << tempLoc->getExpectedYield() << endl;
			if(tempLoc->getState() != 2)
			{
				if(householdLocation->getExpectedYield() < tempLoc->getExpectedYield() && conditionC == true)
				{
					suitableLocations.push_back(tempLoc);
				}
				if(tempLoc->getWater())
				{
					waterSources.push_back(tempLoc);
				}
			}
		}
		if(suitableLocations.size() == 0 || waterSources.size() == 0)
		{
			if(conditionC == true)
			{
				conditionC = false;
			}
			else
			{
				conditionC = true;
				i++;
				if(range*i > boardSizeY)
				{
					removeHousehold(household);
					cout << "\033[1;31mHousehold left because of no suitable location\033[0m\n";
					return false;
				}
			}
			goto LocationSearch;
		}
		else if(suitableLocations.size() == 1)
		{
			std::vector<int> loc2;
			locationSpace->getLocation(suitableLocations[0]->getId(),loc2);
			householdSpace->moveTo(household->getId(),repast::Point<int>(loc2[0], loc2[1]));
			cout << "\033[1;32m1 location available\nHouse moved to (" << loc2[0] << "," << loc2[1] << ")\033[0m\n";
			return true;
		}
		else
		{
			std::vector<int> point1, point2;
			std::vector<double> distances;
			for (std::vector<Location*>::iterator it1 = suitableLocations.begin() ; it1 != suitableLocations.end(); ++it1)
			{
				locationSpace->getLocation((&**it1)->getId(),point1);
				for (std::vector<Location*>::iterator it2 = waterSources.begin() ; it2 != waterSources.end(); ++it2)
				{
					locationSpace->getLocation((&**it1)->getId(),point2);
					double distance = sqrt(pow((point1[0]-point2[0]),2) + pow((point1[1]-point2[1]),2));
					distances.push_back(distance);
				}
			}
			int minElementIndex = std::min_element(distances.begin(),distances.end()) - distances.begin();
			minElementIndex = minElementIndex / waterSources.size();
			std::vector<int> loc2;
			locationSpace->getLocation(suitableLocations[minElementIndex]->getId(),loc2);
			householdSpace->moveTo(household->getId(),repast::Point<int>(loc2[0], loc2[1]));
			cout << "\033[1;32mMultiple locations available\nHouse moved to (" << loc2[0] << "," << loc2[1] << ")\033[0m\n";
			return true;
		}
	// get all water sources in the range
	// find distance
	// choose closest
}

void AnasaziModel::test3(){
	int x,y,i;
	cout << "Enter the Location coordinate x:" << endl;
	cin >> x;
	cout << "Enter the Location coordinate y:" << endl;
	cin >> y;
	cout << "Enter the year:" << endl;
	cin >> i;
	std::vector<Location*> locationList;
	locationSpace->getObjectsAt(repast::Point<int>(x, y), locationList);
	locationList[0]-> printproperties();
	cout << "pdsiGeneral:" << pdsi[i].pdsiGeneral << endl;
	cout << "pdsiNorth:" << pdsi[i].pdsiNorth << endl;
	cout << "pdsiMid:" << pdsi[i].pdsiMid << endl;
	cout << "pdsiNatural:" << pdsi[i].pdsiNatural << endl;
	cout << "pdsiUpland:" << pdsi[i].pdsiUpland << endl;
	cout << "pdsiKinbiko:" << pdsi[i].pdsiKinbiko << endl;
	cout << "hydroGeneral:" << hydro[i].hydroGeneral << endl;
	cout << "hydroNorth:" << hydro[i].hydroNorth << endl;
	cout << "hydroMid:" << hydro[i].hydroMid << endl;
	cout << "hydroNatural:" << hydro[i].hydroNatural << endl;
	cout << "hydroUpland:" << hydro[i].hydroUpland << endl;
	cout << "hydroKinbiko:" << hydro[i].hydroKinbiko << endl;
}
