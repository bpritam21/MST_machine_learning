#include "countnoofnodes.h"

class countnodes{ public:
	static int counter(){
    std::string line;
    std::ifstream myfile("filename.txt");
    while (std::getline(myfile, line))
        ++number_of_nodes;
   	return 0;
}
};