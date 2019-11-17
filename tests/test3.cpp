//Read archaeological data as input and propagate this to the network

#include <stdio.h>
#include <vector>
#include <boost/mpi.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

#include "hydro.csv"
#include "map.csv"
#include "pdsi.csv"
#include "settlement.csv"
//#include "useful data.xlsx"
#include "water.csv"


int main(){
	
	using namespace std;
	const int rows = 552;
	const int cols = 7;
	
	int h[552][7] = {};
	
	ifstream file("hydro.csv");
	
	if (file.is_open()){
		float r[rows][cols];
		
		for (int i = 0; i < rows; i++){
			for (int j = 0; j < cols; j++){
				file >> r[i][j];
				file.get();
				h[i][j] = r[i][j];
			}
		}
	}	
}
