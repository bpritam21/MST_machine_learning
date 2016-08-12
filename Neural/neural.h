#ifndef NEU_H_
#define NEU_H_

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define TRANSPOSE true
#define NOT_TRANSPOSE false
#define FORWARD true
#define BACKWARD false

#define RANDOM_FRACTION		((double)rand()/((double)RAND_MAX+1))
#define RANDOM_INT			((int)rand())
#define TRAIN		1
#define TEST		2
#define EXIT		3

using namespace std;

int num_layers;
int num_inputs;
int num_outputs;
int set;
int io_count;
int* node_count;
double lambda;
double cost;
double old_cost;
double learning_rate;

double** z;
double** a;
double** del;
double** x;
double** y;

double*** theta;
double*** DELTA;
double*** D;

void read_model(char* argv[]);
void read_dataset(char* argv[], int choice);
void load_input();

void make_system();
void initialize_theta();
void initialize_a();
void make_a();
void initialize_z();
void make_z();
void make_DELTA();
void initialize_del();
void initialize_DELTA();
void initialize_D();

void print_theta();

void forward_prop();
void calculate_a(int layer);
double signum(double input);
void matrix_mul(char matrix_sel, int layer, bool flow);

void backward_prop();
void calculate_del(int layers);
void calculate_D();

void partial_cost();
void update_theta();

void update_learning_rate();

void print_result();

#endif