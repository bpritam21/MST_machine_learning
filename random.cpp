#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#define RANDOM_INT		((long int)rand())
#define POINTS_COUNT	59
#define SIZE			100

using namespace std;

int main(int argc, char *argv[]){

	srand((long int)time(NULL));
	long int temp_pointx, temp_pointy;
	long int randx = 0;
	long int randy = 0;
	
	ofstream myfile;
  	myfile.open (argv[1]);
  	myfile << randx <<" "<<randy;
  	
  	for(long int i=0; i<POINTS_COUNT; i++){

  		randx = RANDOM_INT % SIZE;
		randy = RANDOM_INT % SIZE;
		myfile << endl<<randx <<" "<<randy;
	}

  	myfile.close();

  	return 0;
}