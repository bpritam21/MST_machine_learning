#ifndef GEN_H_
#define GEN_H_

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

//#define CROSSOVER_RATE				0.6			//Crossover Rate
//#define MUTATION_RATE				0.1			//Mutation Rate
#define POP_SIZE					1000		//must be an even number
#define MAX_ALLOWABLE_GENERATIONS	400			//Maximum number of generations
#define ZERO						0
#define	NOT_FOUND					-1
#define SINK						0
#define RANDOM_SOL					'1'
#define DETERMINISTIC_SOL			'0'
#define MAX_DOUBLE					999999999999.99

#define RANDOM_FRACTION		((double)rand()/((double)RAND_MAX+1))
#define RANDOM_INT			((int)rand())

using namespace std;

typedef struct chromosome{
	string gene_arr;
	double distance;
	
	chromosome(): gene_arr(""), distance(0.0){};
	chromosome(string sol, double length): gene_arr(sol), distance(length){}
}chrom;


double current_length;
float crossover_rate;
float mutation_rate;
uint64_t chrom_length;
uint64_t gene_length;
uint64_t points_count;
vector< pair < uint64_t, uint64_t > > points;


int calculate_distance(vector<chrom> &sol, double &tot_dist);
vector <chrom> create_initial_solutions();
vector <chrom> weed_out (vector<chrom> intial_sol);
vector <chrom> crossover(vector <chrom> parent_solutions, double tot_dist);
vector <chrom> make_parent(vector <chrom> parent_solutions, vector <chrom> child_solutions, double distance);
vector <int> interpret_solution(chrom sol);
void mutate(vector <chrom> &sol);
void read_file(char* argv[]);
void print_solutions(vector <chrom> sol);
void print_input();
void make_earth();
void update_earth(bool flag);
string random_bit_generator();
string bin(int num);
bool invalid_node(vector <int> chain);
bool check_loop(vector <int> chain);
bool check_duplicate(vector <int> v);
bool check_disjoint(vector <int> input);

#endif