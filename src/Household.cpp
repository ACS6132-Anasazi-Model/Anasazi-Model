#include "Household.h"
#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include <stdio.h>
#include "repast_hpc/Random.h"


Household::Household(repast::AgentId id, int deAge) //for init
{
  householdId = id;
  age = 0;
  deathAge = deAge;
}

Household::~Household()
{

}

/* Getters specific to this kind of Agent */

int Household::checkMaize()
{
  if(assignedField->getExpectedYield() > 800)
  {
    maizeSatisfied = true;
  }
  else
  {
    maizeSatisfied = false;
  }
}

bool Household::death()
{
  if(age>=deathAge)
  {
      return true;
  }
  else
  {
    return false;
  }
}

bool Household::fission(int fissionAge, int gen)
{
  if(age>fissionAge && gen == 0)
  {
      return true;
  }
  else
  {
    return false;
  }
}

void Household::nextYear()
{
  age++;
}

repast::Point<int> Household::chooseField()
{
  return repast::Point<int> (0, 0);
}

/* Actions */
void move(repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* space)
{

}
