// #include "Model.cpp"

#include <fstream>

//REPLACE list a WITH model.listOfHousehold
int a[5] = {10,20,30,40,50};

int main(){
	int houseNumber[sizeof(a)/sizeof(a[0])][2];
	for (int i = 0; i < sizeof(a)/sizeof(a[0]); i++){
		houseNumber[i][0] = i;
		houseNumber[i][1] = a[i];
	}

	std::ofstream out ("test6.csv");

	for (int i = 0; i < sizeof(a)/sizeof(a[0]); i++){
		out << houseNumber[i][0] << ", " << houseNumber[i][1];
		out << std::endl;

	}

	out.close();

}
