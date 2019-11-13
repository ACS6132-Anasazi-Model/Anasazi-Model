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
