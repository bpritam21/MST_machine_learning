#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "main.h"
#include "countnoofnodes.cpp"
#include "BPRIM.cpp"
#include "BRBC.cpp"
//#include "extensions.cpp"

countnodes countnoofnodes;

int main(){
	
countnoofnodes.counter();
cout << "enter the user_defined_value epsilon \n" ;
cin >> E ;
/*cout << "Enter the algorithm to be implemented in BLOCK letters  \n";
string Algorithm;
cin >> Algorithm;
if(Algorithm == "BPRIM")
{*/
H1=1;
H2=0;
node = new list [number_of_nodes];
BPRIM();

//H_extensions();

cout << "maximum radius is " << maximum_radius << " units \n"; 
cout << "(1+E)*R is " << radius_bound << " units \n"; 

cout << "wirelength is " << wirelength << " units \n"; 
cout << number_of_nodes << "is the no of nodes " ; 
/*}
else if(Algorithm == "BRBC")
{

node = new list [number_of_nodes];
BRBC();
}
*/
} 

