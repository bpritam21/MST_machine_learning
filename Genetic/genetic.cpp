#include "genetic.h"

int main(int argc, char *argv[]){

	if(argc < 3){
		cout<< "Invalid number of arguments"<<endl;
		exit(1);
	}

	//seed the random number generator with time
	srand((int)time(NULL));
	
	int genTracker = ZERO;	//Tracks the number of generations
	int min_index;
	int up_count = ZERO, same_count = ZERO;
	double tot_dist;
	chrom min_sol;
	vector <chrom> parent_solutions, child_solutions;		//Declare a solution space
	
	read_file(argv);		//Read the file to get the nodes and the specifications

	print_input();			//Print what it has read
	
	//Find the length of a chromosome
	chrom_length=0;
	gene_length=0;
	for(int i=points_count; i>0;){
		i = i/2;
		gene_length++;
	}
	chrom_length=points_count*gene_length;

	//cout<<endl<<"Gene Length: "<<gene_length<<endl;
	//cout<<"Number of input points: "<<points_count<<endl;
	//cout<<"Chromosome Length: "<<chrom_length<<endl<<endl;
	
	//infinite loop - hahahahahahahahahahaha
	if(argv[2][0] == RANDOM_SOL && argv[2][1] == '\0'){

		while(parent_solutions.size()<1 && genTracker<10000){
			genTracker++;
			vector <chrom> temp_solutions;		//Declare a solution space
			
			//Step - 1: Create a few valid initial solutions
			temp_solutions = create_initial_solutions();
			
			//cout<<"Solutions before weeding:"<<endl<<endl;
			//print_solutions(temp_solutions);		//Print the solution
			
			temp_solutions = weed_out(temp_solutions);

			for(int j=0; j< temp_solutions.size(); j++)
				parent_solutions.push_back(temp_solutions[j]);

			//cout<<endl<<"Solutions after weeding:"<<endl<<endl;
			//print_solutions(parent_solutions);		//Print the solutions
		}
		//cout<< endl<< "Found in generation: "<<genTracker<<endl;
	
	}else if(argv[2][0] == DETERMINISTIC_SOL && argv[2][1] == '\0'){
		
		for(int j=1; j<=2; j++){
			string temp_str = bin(SINK);
			chrom temp_chrom;

			for(int i=1; i<points_count; i++){
				
				temp_str += bin((i+j) % (points_count-1));
			}

			temp_chrom.gene_arr = temp_str;
			temp_chrom.distance = 0.0;

			parent_solutions.push_back(temp_chrom);
		}

		parent_solutions = weed_out(parent_solutions);
		
	}else{

		while(parent_solutions.size()<2){
			
			cout<< endl<< "Enter a valid initial solution that you want to consider (one node at a time) (1-N) (0 is for SINK):"<<endl;
			
			string temp_str = bin(SINK);
			chrom temp_chrom;

			for(int i=0; i<points_count-1; i++){
				int temp;
				
				fflush(stdin);
				cout<<"Node "<<i+1<<" --> ";
				cin>>temp;
				temp_str += bin(temp);
			}


			temp_chrom.gene_arr = temp_str;
			temp_chrom.distance = 0.0;

			parent_solutions.push_back(temp_chrom);
			parent_solutions = weed_out(parent_solutions);
		}
	}

	min_index = calculate_distance(parent_solutions,tot_dist);
	min_sol = parent_solutions[min_index];

	for(int i=0; i<parent_solutions.size();i++)
		//cout <<i<<"-th distance is: "<<parent_solutions[i].distance<<endl;

	//cout<< "Minimum Index: "<<min_index<<endl;

	genTracker = 0;

	while(up_count<600 && same_count<100 ){//&& genTracker < MAX_ALLOWABLE_GENERATIONS){
		//bool up_flag = false, same_flag = false;
		//cout<< genTracker <<"-th child generation ";
		make_earth();
		
		child_solutions = crossover(parent_solutions,tot_dist);
		mutate(child_solutions);

		child_solutions = weed_out(child_solutions);

		if(!child_solutions.size())
			continue;
		
		min_index = calculate_distance(child_solutions,tot_dist);

		if(child_solutions[min_index].distance > min_sol.distance){
			up_count++;
			//up_flag = true;
		
		}else if(child_solutions[min_index].distance == min_sol.distance){
			same_count++;
			//same_flag = true;

		}else{
			up_count = ZERO;
			same_count = ZERO;
			min_sol = child_solutions[min_index];
		}

		parent_solutions = make_parent(parent_solutions, child_solutions, min_sol.distance);

		//cout<< " total distance: "<<tot_dist <<"   Minimum distance: "<<child_solutions[min_index].distance<<endl;
		genTracker++;
	}

	
	// cout<<"\nTree is: ";

	// vector <int> nodes = interpret_solution(min_sol);
	// for(int j=0; j< nodes.size(); j++)
	// 	cout<<nodes[j]<< " ";
	// //cout<<endl;

	cout<<min_sol.distance<<endl;
}


vector <chrom> make_parent(vector <chrom> parent_solutions, vector <chrom> child_solutions, double distance){
	vector <chrom> ret;
	
	double accepted_distance = distance *(1.2);
	int i=0;

	
	while(ret.size()<POP_SIZE && i<child_solutions.size()){
		
		if(child_solutions[i].distance <= accepted_distance)
			ret.push_back(child_solutions[i]);

		i++;
	}

	i=0;

	while(ret.size()<POP_SIZE && i<parent_solutions.size()){
		
		if(parent_solutions[i].distance <= accepted_distance)
			ret.push_back(parent_solutions[i]);

		i++;
	}

	return ret;
}


//Fixes the initial starting rates
void make_earth(){

	//do{
		mutation_rate = (float) RANDOM_FRACTION/10.0;
		crossover_rate = (float) RANDOM_FRACTION;

		//cout<<"Mutation rate: "<<mutation_rate<<" Crossover Rate: "<<crossover_rate;
	//}while(mutation_rate > 0.01 && crossover_rate > 0.5);
}

//Updates the rate depending on the current trend
void update_earth(bool flag){
	
	if(flag == true){
		
		mutation_rate += (float) RANDOM_FRACTION * mutation_rate;
		crossover_rate += (float) RANDOM_FRACTION * crossover_rate;
	}
	else{

	}
}

//Mutates the solution space
void mutate(vector <chrom> &sol){

	for(int i=0; i< sol.size();i++){
		
		for(int j=0;j<sol[i].gene_arr.length(); j++){
			
			if(RANDOM_FRACTION < mutation_rate){
				
				if(sol[i].gene_arr.at(j) == '1')
					sol[i].gene_arr.at(j) = '0';
				else
					sol[i].gene_arr.at(j) = '1';
			}
		}
	}
}


//Returns the child solutions from crossover
vector <chrom> crossover(vector <chrom> parent_solutions, double tot_dist){
	
	vector <chrom> child;
	int dad_index=0, mom_index=0;
	int increment = 0;
	chrom dad, mom;//, child_str;
	double acceptance_rate=0.0;

	
	while(acceptance_rate < 0.4)
		acceptance_rate = RANDOM_FRACTION;

	//cout<<" start accept rate "<<acceptance_rate;

	while(child.size()<POP_SIZE){

		chrom temp_child1, temp_child2;
		increment = 0;

		do{
			dad_index = RANDOM_INT % parent_solutions.size();
			mom_index = RANDOM_INT % parent_solutions.size();

			dad = parent_solutions[dad_index];
			mom = parent_solutions[mom_index];

			increment++;
			if(increment > 100)
				acceptance_rate = acceptance_rate*2.0;
		
		}while((double) (dad.distance/tot_dist) > acceptance_rate && (double) (mom.distance/tot_dist) > acceptance_rate);
		
		if (RANDOM_FRACTION < crossover_rate){

			int crossover_index = (int) (RANDOM_FRACTION * chrom_length);
			
			temp_child1.gene_arr = dad.gene_arr.substr(ZERO, crossover_index) + mom.gene_arr.substr(crossover_index, chrom_length-crossover_index);
			temp_child2.gene_arr = mom.gene_arr.substr(ZERO, crossover_index) + dad.gene_arr.substr(crossover_index, chrom_length-crossover_index);
		}

		child.push_back(temp_child1);
		child.push_back(temp_child2);
	}

	//cout<<" end accept rate "<<acceptance_rate;

	return child;
}

//Returns the index of the minimal distance spanning tree in the solution space and fills the others with their distance
int calculate_distance(vector<chrom> &sol, double &tot_dist){
	double dist = MAX_DOUBLE;
	int ret = -1;
	tot_dist = ZERO;

	for(int i=0; i<sol.size(); i++){
		vector <int> chain = interpret_solution(sol[i]);
		double temp_dist=0.0;

		for(int j=1; j< chain.size(); j++){
			if(chain[j]== SINK){
				continue;
			}

			int x1 = points[j].first, y1 = points[j].second;
			int x2 = points[chain[j]].first, y2 = points[chain[j]].second;
			
			temp_dist += (double) sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
		}

		sol[i].distance = temp_dist;
		tot_dist += sol[i].distance;

		ret = (dist > temp_dist)? i: ret;
		dist = (dist > temp_dist)? temp_dist: dist;
	}

	return ret;
}

//Returns binary string of a number
string bin(int num){
	string ret = "";
	int temp = points_count;
	
	do{

		ret = ((num%2 == 1)?"1":"0") + ret;
		num = num/2;
		temp = temp/2;

	}while(temp>0);

	return ret;
}


//Function to weed out non-feasible solutions
vector <chrom> weed_out (vector<chrom> initial_sol){
	
	vector <chrom> ret;

	for(int i=0; i< initial_sol.size();i++){
		
		vector <int> chain = interpret_solution(initial_sol[i]);
		
		if(!chain.size()){
			continue;
		}

		if(!invalid_node(chain) && !check_disjoint(chain) && !check_loop(chain)){
			ret.push_back(initial_sol[i]);
		}
	}

	return ret;
}


//Checks if there are any of the nodes are connected to any invalid nodes
bool invalid_node(vector <int> chain){
	
	for (int i=1; i<chain.size(); i++){
		if(chain[i] >= points_count){
			return true;
		}
	}

	return false;
}


//checks the presence of disjoint trees
bool check_disjoint(vector <int> chain){
	bool flag = false;

	for (int i=1; i<chain.size(); i++){
		if(chain[i] == SINK){
			if(flag == false)
				flag = true;
			else
				return true;
		}
	}

	return false;
}

//Checks for loop in the chromosome length - if present then return 'TRUE' or else return 'FALSE'
bool check_loop(vector <int> input){
	
	vector <int> chain = input;
	vector <int> path;
	int i = 1;
	int j = i;

	while (i<chain.size()){
		int temp;

		if(chain[j] == j){		//Check self-loop
			return true;
		}

		if(chain[j] == SINK){
			path.push_back(j);
			if(check_duplicate(path))
				return true;
			else
				i++;
			j=i;
			path.clear();
			continue;
		}
		
		path.push_back(j);
		temp = chain[j];
		chain[j] = SINK;
		j=temp;
	}
	return false;
}

//Function to check if there is any duplicate character in a string
bool check_duplicate(vector <int> v){

	if(v.size()==0)
		return false;

	for(int i=0; i< v.size()-1; i++){
		for(int j=i+1; j< v.size();j++){
			if(v[i] == v[j])
				return true;
		}
	}

	return false;
}

//Function to print the inputs
void print_input(){

	//cout<<"The inputs are:"<<endl;

	// for(int i=0; i<points.size(); i++){
	// 	cout<<points[i].first<<" "<<points[i].second<<endl;
	// }
}

//Function to read file and store the values
void read_file(char* argv[]){
	
	ifstream myfile;
	myfile.open(argv[1]);
	points_count = 0;
	
	while(!myfile.eof()){
		pair<uint64_t, uint64_t> temp_point;
		
		myfile >> temp_point.first;
		myfile >> temp_point.second;
		
		points.push_back(temp_point);
		points_count++;
	}
	points_count++;	//The sink node
}

//Function to create a random set of initial solution
vector <chrom> create_initial_solutions(){
	
	vector <chrom> ret;

	for (int i=0; i<POP_SIZE; i++){

		chrom temp_sol;
		
		temp_sol.gene_arr = "";	//Declare an empty string for every member of the population
		temp_sol.distance = 0.0;
		
		for(int j=0; j<chrom_length; j++)		//Populate the gene for every member
			temp_sol.gene_arr += random_bit_generator();

		ret.push_back(temp_sol);
	}

	return ret;
}

//Function to generate random bit value
string random_bit_generator(){
	
	if (RANDOM_FRACTION > 0.5)
		return "1";
	else
		return "0";
}

//Function to display the solution
void print_solutions(vector <chrom> sol){
	
	//int dist=0;
	chrom temp;
	vector <int> nodes;

	for (int i=0; i<sol.size(); i++){
		
		//cout<<i<<"-th solution is:"<<endl;
		temp = sol[i];
		nodes = interpret_solution(temp);
		// for(int j=0; j< nodes.size(); j++)
		// 	cout<<nodes[j]<< " ";
		// cout<<endl;
		// //cout<<sol[i].gene_arr<<endl;
	}

	//cout<<endl<<"Exiting for loop"<<endl;
}


vector <int> interpret_solution(chrom sol){

	vector <int> ret;
	string temp="";

	for(int i=0; i< sol.gene_arr.length(); i += gene_length){
		
		int num=0;
		temp = sol.gene_arr.substr(i,gene_length);		//Extracts each gene from the chromosome

		//Converts the binary to decimal
		for(int j=0; j< gene_length; j++){
			num*=2;
			if (temp.at(j) == '1'){
				num += 1;
			}
		}

		ret.push_back(num);
	}

	return ret;
}