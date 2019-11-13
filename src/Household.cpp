#include "Household.h"
#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include <stdio.h>
#include "repast/Random.h"


Household::Household(repast::AgentId id, int deathAge) //for init
{
  householdId = id;
  householdType = 1
  age = 0;
  deathAge = deathAge;
}

~Household::Household()
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
  if(age>fissionAge && gen == 0))
  {
      return true;
  }
  else
  {
    return false;
  }
}

void nextYear()
{
  age++;
}

repast::Point Household::chooseField()
{
  return repast::Point(0, 0)
}

/* Actions */
void move(repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* space);
};
