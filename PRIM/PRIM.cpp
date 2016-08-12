#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

void read_file(char* argv[]);
double find_distance(int node_num1, int node_num2);
void print_adj();
void generate_adj_matrix();
void print_sol();
string bin_str(int i);

vector < pair < int, double > > graph;
vector< pair < int, int > > points;
int points_count;
int min_node;
double min_dist;
bool** adj_matrix;


int main(int argc, char* argv[]){

	if(argc<2){
		cout<< "Invalid number of parameters"<<endl;
		exit(1);
	}

	read_file(argv);
	bool* done_list;
	pair < int, double > temp_node;

	done_list = (bool*) calloc (points_count,sizeof(bool));

	min_node = 0;
	
	temp_node.first = 0;
	temp_node.second = 0.0;
	graph.push_back(temp_node);

	done_list[0]=true;

	for(int j=1; j < points_count; j++){
		
		for (int i=0; i < points_count; i++){

			if(done_list[i]==true)
				continue;

			double temp_dist = find_distance(min_node,i);

			temp_node.second = temp_dist;
			temp_node.first = min_node;

		//	cout<< "Temp dist: "<<temp_dist<<endl;
	//		cout<< "Min node: "<<min_node<<endl;

			if(j==1){
				graph.push_back(temp_node);
			}
			else if(graph[i].second > temp_dist){
				graph[i] = temp_node;
			}

		}

		double min_distance = 999999999.99999;

		for(int i=1; i<points_count; i++){
			if(done_list[i]==true)
				continue;

			if(graph[i].second < min_distance){
				min_node = i;
				min_distance = graph[i].second;
			}
		}

		done_list[min_node]=true;
	}

	print_sol();
	//print_adj();
	//generate_adj_matrix();

	return 0;
}

void print_sol(){
	// for(int i=0; i<points_count; i++){
	// 	cout<< points[i].first<<" "<<points[i].second;
	// 	if(i!=points_count-1){
	// 		cout<<" ";
	// 	}


	// }

	// string str;

	// for (int i = 1; i < points_count; ++i){
	// 	str = bin_str(i);
	// 	cout<<str;
	// }

	double tot_distance=0.0;

	for(int i=0; i<points_count; i++){
		tot_distance += graph[i].second;
	}
	cout<<tot_distance<<endl;
}

string bin_str(int i){
	string ret = "";
	int num = graph[i].first;
	int temp = points_count;

	//cout<<endl<<num<<"   "<<temp<<endl;

	
	do{

		ret = (((int) num%2 == (int) 1)?" 1":" 0") + ret;
		num = (int) num/2;
		temp = (int) temp/2;

	}while(temp>0);

	return ret;
}

void print_adj(){
	
	double tot_distance=0.0;

	cout<< "Node1\t-\tNode2"<<endl;

	for(int i=0; i<points_count; i++){
		cout<< i<< "\t-\t" << graph[i].first<<endl;
		tot_distance += graph[i].second;
	}
	cout<<endl<<"Total Distance: "<<tot_distance<<endl;
}

void generate_adj_matrix(){
	
	adj_matrix = (bool**) malloc(points_count*sizeof(bool*));

	for(int i=0; i<points_count;i++){
		adj_matrix[i] = (bool*) calloc(points_count,sizeof(bool));
	}

	for(int i=0; i<points_count; i++){
		adj_matrix[i][graph[i].first]=true;
		adj_matrix[graph[i].first][i]=true;
	}

	for(int i=0;i<points_count; i++){
		for(int j=0;j<points_count; j++){
			cout<< adj_matrix[i][j]?"1":"0";
			cout<<"\t";
		}
		cout<<endl;
	}
}

double find_distance(int node_num1, int node_num2){

	int x1 = points[node_num1].first, y1 = points[node_num1].second;
	int x2 = points[node_num2].first, y2 = points[node_num2].second;

	double temp_dist;
	temp_dist = (double) sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
	
	return temp_dist;
}

void read_file(char* argv[]){
	
	ifstream myfile;
	myfile.open(argv[1]);
	points_count = 0;
	
	do{
		pair<int, int> temp_point;
		
		myfile >> temp_point.first;
		myfile >> temp_point.second;
		
		points.push_back(temp_point);
		points_count++;
	
	}while(!myfile.eof());

//	points_count++;	//The sink node

	//cout<< "Points count: "<<points_count<<endl;
}