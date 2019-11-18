#include <stdio.h>
#include <vector>
#include <boost/mpi.hpp>

#include "model.props"


int main(){
	max.age=30;
	cout<<"max.age = "<<max.age<<endl;
	max.age=25;
	cout<<"max.age (smaller) = "<<max.age<<endl;
	max.age=35;
	cout<<"max.age (larger) = "<<max.age<<endl;
}
