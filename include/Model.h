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
	int boardSize;
  std::vector<Household> listOfHousehold;

	repast::Properties* props;
	repast::SharedContext<Agent> context;
  //Need to confirm this line
	repast::SharedDiscreteSpace<Agent, repast::StrictBorders, repast::SimpleAdder<Agent> >* discreteSpace;

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
