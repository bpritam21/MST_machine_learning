#include "main.h"
#include "readfromfile.h"
#include <cstdlib> 


int readfromfile(list* node)
	{
	/*string fileName;
	cout << "enter the filename\n";
	cin>> fileName;*/
	
	//ifstream myfile(fileName);	// input file has integers, two per line
	ifstream myfile("filename.txt");
	{
		if (myfile.is_open())
		{
		int i=0;
		while (!myfile.eof() && i<=number_of_nodes)
		{
			myfile >> node[i].xcoordinate;
			myfile >> node[i].ycoordinate;
			i++;
		}
		
		for (int i=0; i< number_of_nodes; i++)
		{
		node[i].radius = abs((node[i].xcoordinate)-(node[0].xcoordinate)) + abs((node[i].ycoordinate)-(node[0].ycoordinate)); 
		}
		
		for (int i= 0; i< number_of_nodes; i++)
		{
		if(node[i].radius > maximum_radius)
		{
			maximum_radius = node[i].radius;
		}
		}

		}
	
	myfile.close();
	}
	
	radius_bound = (1+E)* maximum_radius ; 
	cout << "radius bound is " << radius_bound << "\n" ;
	
	
return 0; 
}
