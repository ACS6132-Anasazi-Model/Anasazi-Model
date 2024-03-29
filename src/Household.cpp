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

bool Household::checkMaize(int needs)
{
  if((assignedField->getExpectedYield() + maizeStorage) > needs)
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

bool Household::fission(int minFissionAge, int maxFissionAge, double gen, double fProb)
{
  if((age>=minFissionAge && age<=maxFissionAge) && (gen <= fProb))
  {
      return true;
  }
  else
  {
    return false;
  }
}

void Household::nextYear(int needs)
{
  age++;
  maizeStorage = assignedField->getExpectedYield() + maizeStorage - needs;
}

void Household::chooseField(Location* Field)
{
  assignedField = Field;
}

/* Getters for test1 */

//repast::AgentId Household::getHouseholdId();
//Location* Household::getAssignedField();




/* Actions */
void move(repast::SharedDiscreteSpace<Household, repast::StrictBorders, repast::SimpleAdder<Household> >* space)
{

}
