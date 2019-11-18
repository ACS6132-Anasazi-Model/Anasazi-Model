//Read archaeological data as input and propagate this to the network

#include <stdio.h>
#include <vector>
#include <boost/mpi.hpp>

#include <string>
#include <Model.h>

/*
#include <fstream>
#include <sstream>
#include <iostream>
*/

//int a[2][7] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};

//Prints hydro array from Model.cpp to show archaeological data is in network
int main(){

	for (int i = 0; i < sizeof(hydro)/sizeof(hydro[0]); i++){
		for (int j = 0; j < sizeof(hydro[0])/sizeof(int); j++){
		std::cout << a[i][j] << ", ";
		}

		std::cout << std::endl;
	}

}
