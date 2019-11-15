#ifndef MODEL
#define MODEL

#include <boost/mpi.hpp>
#include "repast_hpc/Schedule.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/GridComponents.h"
#include "repast_hpc/Random.h"

#include "Household.h"

class AnasaziModel{
private:
  int year;
  int stopAt;
	int boardSizeX, boardSizeY, procX, procY, bufferSize;
  int randomSeed;
  int houseID = 0;
  //std::vector<Household> listOfHousehold;
  struct Parameters
  {
    int startYear;
    int endYear;
    int maxStorageYear;
    int householdNeed;
    int minFissionAge;
    int maxAge;
    int mostLikelyDeathAge;
    int maxDistance;
    int initMinCorn;
    int initMaxCorn;
    double annualVariance;
    double spatialVariance;
    double fertilityProbability;
    double harvestAdjustment;
  } param;

	repast::Properties* props;
	repast::SharedContext<Household> context;
  repast::SharedContext<Location> locationcontext;  //Need to confirm this line
	repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* householdSpace;
  repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >* locationSpace;
  repast::DoubleUniformGenerator fissionGen = repast::Random::instance()->createUniDoubleGenerator(0,1);
  repast::TriangleGenerator deathAgeGen = repast::Random::instance()->createTriangleGenerator(0,1,2);

public:
	AnasaziModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm);
	~AnasaziModel();
	void initAgents();
	void initSchedule(repast::ScheduleRunner& runner);
	void doPerTick();
  bool fissionHousehold();
  bool removeHousehold();
  int countHousehold();
  void readcsv1(repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >* locationSpace);
  void readcsv2(repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* householdSpace);
  void readcsv3(repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >* locationSpace);
  void readcsv4(repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >* locationSpace);
  void readcsv5(repast::SharedDiscreteSpace<Location, repast::StrictBorders, repast::SimpleAdder<Location> >* locationSpace);
};

#endif
