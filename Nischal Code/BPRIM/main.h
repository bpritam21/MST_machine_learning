#ifndef Main_H
#define Main_H

#include "countnoofnodes.h"

int maximum_radius;
float radius_bound;
float E; 
int wirelength;
int MST=0;
string extension;
int H1;
int H2;


class list{
public:
int xcoordinate;
int ycoordinate;
int radius;
};

list *node;

extern int number_of_nodes;

#endif

