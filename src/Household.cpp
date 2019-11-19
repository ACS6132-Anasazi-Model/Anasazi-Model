#include "Household.h"
#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedDiscreteSpace.h"
#include <stdio.h>
#include "repast_hpc/Random.h"


Household::Household(repast::AgentId id, int a, int deAge, int mStorage) //for init
{
  householdId = id;
  age = a;
  deathAge = deAge;
  maizeStorage = mStorage;
  assignedField = NULL;
}

Household::~Household()
{

}

/* Getters specific to this kind of Agent */

int Household::splitMaizeStored(int percentage)
{
  int maizeEndowment;
  maizeEndowment = maizeStorage * percentage;
  maizeStorage = maizeStorage - maizeEndowment;
  return maizeEndowment;
}

bool Household::checkMaize()
{
  if((assignedField->getExpectedYield() + maizeStorage) > 800)
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Household::death()
{
  #ifdef DEBUG
    cout << "ID:" << householdId.id() << ", Age = " << age << ", Death Age = " << deathAge << endl;
  #endif

  if(age>=deathAge)
  {
      return true;
  }
  else
  {
    return false;
  }
}

bool Household::fission(int minFissionAge, int maxFissionAge, double gen)
{
  if(age>=minFissionAge && age<=maxFissionAge && gen >= 0.875)
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

void Household::chooseField(Location* Field)
{
  assignedField = Field;
}

/* Actions */
void move(repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* space)
{

}
