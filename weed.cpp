#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define POINTS_COUNT 60

int main(int argc, char *argv[]){

	int temp_pointx[POINTS_COUNT], temp_pointy[POINTS_COUNT];

	ifstream myinfile;
	myinfile.open(argv[1]);
	long int input=0;

	while(!myinfile.eof()){
		
		//fflush(myinfile);

		myinfile >> temp_pointx[input];
		myinfile >> temp_pointy[input];
		
		for(long int i=0; i<input; i++){

			//cout<<i<<" "<< temp_pointx[i]<<" "<<temp_pointy[i]<<" "<<temp_pointx[input]<<" "<<temp_pointy[input]<<endl;

		  	if(temp_pointy[input] == temp_pointy[i] && temp_pointx[input] == temp_pointx[i]){
		  		//cout<<"MATCH"<<endl;
				exit(1);
			}
		}

		input++;

	}

	myinfile.close();
	
	exit(0);
}