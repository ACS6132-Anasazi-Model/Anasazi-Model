#ifndef MODEL
#define MODEL

#include <boost/mpi.hpp>
#include "repast_hpc/Schedule.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/GridComponents.h"

#include "Household.h"

class AnasaziModel{
private:
  int stopAt;
	int boardSizeX, boardSizeY, procX, procY, bufferSize;
  int randomSeed;
  std::vector<Household> listOfHousehold;
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
  }

	repast::Properties* props;
	repast::SharedContext<Agent> context;
  //Need to confirm this line
	repast::SharedDiscreteSpace<Agent, repast::StrictBorders, repast::SimpleAdder<Agent> >* discreteSpace;

  repast::DoubleUniformGenerator fissionGen;
  repast::TriangleGenerator deathAgeGen;

public:
	AnasaziModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm);
	~AnasaziModel();
	void initAgents();
	void initSchedule(repast::ScheduleRunner& runner);
	void doPerTick();
  bool fissionHousehold();
  bool removeHousehold();
  int countHousehold();
  void readcsv1();
};

#endif
