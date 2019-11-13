// #include "Model.cpp"

void Test6::getHousehold(){
	int houseNumber[sizeof(model.listOfHousehold),2] = {};
	for (int i = 0; i < sizeof(model.listOfHousehold); i++){
		houseNumber[i][0] = i;
		houseNumber[i][1] = Model.listOfHousehold[i];
	}

	std:ofstream out("test6.csv");

	for (int i = 0; i < sizeof(model.listOfHousehold); i++){
		outfile << houseNumber[i][0] << ", " << houseNumber[i][1];
		outfile << endl;

	}

	outfile.close();

}
