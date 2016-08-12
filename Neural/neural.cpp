#include "neural.h"

int main(int argc, char *argv[]){

	srand((long int)time(NULL));
	
	if(argc < 4){
		cout<<"Invalid number of parameters"<<endl;
		cout<<"The format is: ./neural [path-to-model] [path-to-training-set] [path-to-testing-set]"<<endl;
		exit(1);
	}

	read_model(argv);

	make_system();
	cout<<endl<<"--------------------------------------Initial theta: ---------------------------------------"<<endl;
	print_theta();
	cout<<endl<<endl;
//	print_theta();

	int choice = TRAIN;

	do{

		cout<<"Enter a valid choice: (1-TRAIN or 2-TEST or 3-EXIT)"<<endl;
		fflush(stdin);
		scanf("%d",&choice);

		if(choice == TRAIN || choice > EXIT){
			read_dataset(argv,TRAIN);
			int iter=0;

			//while(old_cost > 0.01 || (choice > EXIT && iter < choice-EXIT)){

				//for(io_count=0; io_count<set; io_count++){

					io_count=0;
					load_input();

					initialize_DELTA();
					
					forward_prop();

					//cout<< "@@@@@@@@@@@@@@@      Results"<<endl;
					//print_result();

					partial_cost();

					backward_prop();

					calculate_D();

					update_theta();
				//}

				update_learning_rate();
				iter++;
			//}
			cout<<endl<<"-----------------------------Final theta:--------------------------------------"<<endl;
			print_theta();
		}
		else if(choice==TEST){
			read_dataset(argv,TEST);
			
			for(io_count=0; io_count<set; io_count++){
				cost =0.0;
				
				initialize_z();
				initialize_a();
				load_input();
				forward_prop();
				partial_cost();
				print_result();
			}

		}

		// cout<<endl<<"Final theta:"<<endl;
		// print_theta();

		
	}while(choice !=EXIT);
	
	return 0;
}

void print_result(){
	for(int i=1; i<node_count[0]; i++){
		cout<<a[0][i]<<" ";
	}

	for(int i=0; i<node_count[num_layers-1]; i++){
		cout<<" "<<a[num_layers-1][i];
	}
	cout<<"   Cost: "<<cost;
	cout<<endl;
}

void read_model(char* argv[]){

	ifstream myfile;
	myfile.open(argv[1]);
	
	while(!myfile.eof()){
		
		myfile >> set;
		myfile >> num_layers;
		myfile >> lambda;
		myfile >> learning_rate;
		myfile >> num_inputs;
		myfile >> num_outputs;
		node_count = (int*) calloc(num_layers,sizeof(int));

		for(int i=0; i<num_layers; i++){
			myfile >> node_count[i];
		}
	}
}

void read_dataset(char* argv[], int choice){

	ifstream myfile;
	myfile.open(argv[choice+1]);
	
	io_count = 0;
	x = (double**) malloc(set*sizeof(double*));
	y = (double**) malloc(set*sizeof(double*));

	while(!myfile.eof()){
		
		x[io_count] = (double*) calloc(num_inputs,sizeof(double));

		for(int i=0; i<num_inputs; i++){
			myfile >> x[io_count][i];
		}

		y[io_count] = (double*) calloc(num_outputs,sizeof(double));
		
		for(int i=0; i<num_outputs;i++){
			myfile >> y[io_count][i];
		}

		io_count++;
	}
}

void partial_cost(){

	for(int i =0; i < num_outputs; i++){
		cost = (double) cost + (-y[io_count][i]*log(a[num_layers-1][i]) - (1 - y[io_count][i])*log(1 - a[num_layers-1][i]));
	}
}

void update_learning_rate(){
	
	cost = (double) cost/set;
	
	double regulation_factor=0.0;

	for(int l=0; l<num_layers-1; l++){
		for(int i=0; i<node_count[l+1]-1; i++){
			for(int j=0; j<node_count[l]; j++){
				
				regulation_factor += theta[l][i][j]*theta[l][i][j];
			}
		}
	}

	regulation_factor = (double) (lambda*regulation_factor)/(2.0*set);

	cost += regulation_factor;

	if(old_cost > cost){

		learning_rate = 1.1*learning_rate;
	}else{

		learning_rate = 0.5*learning_rate;
	}

	old_cost = cost;
}

void update_theta(){
	for(int l=0; l<num_layers-1; l++){
		for(int i=0; i<node_count[l+1]-1; i++){
			for(int j=0; j<node_count[l]; j++){
				
				theta[l][i][j] = theta[l][i][j] - learning_rate*D[l][i][j];
			}
		}
	}
}


void calculate_D(){
	for(int l=0; l<num_layers-1; l++){
		for(int i=0; i<node_count[l+1]-1; i++){
			for(int j=0; j<node_count[l]; j++){
				
				D[l][i][j] = (double) DELTA[l][i][j]/set;
					if(j>0)
						D[l][i][j] += (double) lambda*theta[l][i][j];
			}
		}
	}
}


void make_system(){

	initialize_theta();
	make_z();
	initialize_z();
	make_a();
	initialize_del();
	make_DELTA();
	initialize_D();
	old_cost = 99999999.9999;
}

void initialize_D(){
	D = (double***) malloc((num_layers-1)*sizeof(double**));

	for(int l=0; l<num_layers-1; l++){
		D[l] = (double**) malloc((node_count[l+1]-1)*sizeof(double*));

		for(int i=0; i<node_count[l+1]-1; i++){
			D[l][i] = (double*) calloc(node_count[l],sizeof(double));
		}
	}
}


void forward_prop(){

	cout<<"inside forward_prop"<<endl;
	
	for(int l=0; l<num_layers-1; l++){
		matrix_mul('z',l,FORWARD);
		calculate_a(l+1);
	}
}


void calculate_a(int layer){

	for(int i=1; i<node_count[layer];i++){
		a[layer][i] = signum(z[layer][i]);
	}

	if(layer==num_layers-1)
		a[layer][0] = signum(z[layer][0]);

	cout<<"-------------- A["<<layer<<"]------------------"<<endl;
	for(int i=0; i<node_count[layer];i++){
		cout<<a[layer][i]<<" ";
	}
	cout<<endl;
}


double signum(double input){
	double x = -input;
	double out;

	x = 1 + exp(x);
	out = 1/x;
	return out;
}

void matrix_mul(char matrix_sel, int layer, bool flow){
	g++
	int limit;

	if(flow == FORWARD){

		if(layer==num_layers-2){
			limit = node_count[layer+1];
			z[layer+1][0] = 1.0;
		}
		else{
			limit = node_count[layer+1]-1;
			z[layer+1][0] = 1.0;
		}


		for(int i=0;i<limit; i++){
			for(int j=0; j<node_count[layer]; j++){

				z[layer+1][i+1] += theta[layer][i][j]*a[layer][j];
			}
		}

	}else{

		if(matrix_sel == 'd'){
			
			double* temp = (double*) calloc(node_count[layer],sizeof(double));

			for(int i=0;i<node_count[layer]; i++){
				for(int j=0; j<node_count[layer+1]-1; j++){
					
					del[layer][i] = theta[layer][j][i]*del[layer+1][j+1];
				}
			}
			
			for(int i=0; i<node_count[layer];i++)
				temp[i] = a[layer][i]*(1.0 - a[layer][i]);

			for(int i=0;i<node_count[layer];i++)
				del[layer][i] *= temp[i];
		}

		if(matrix_sel == 'D'){

			for(int i=0; i<node_count[layer+1]-1; i++){
				for(int j=0; j<node_count[layer]; j++){
					
					DELTA[layer][i][j] += del[layer+1][i+1]*a[layer][j];
				}
			}
		}

	}
}



void backward_prop(){
	for(int l=num_layers-1; l>=0; l--){
		calculate_del(l);
	}
}

void calculate_del(int layer){
	
	if(layer == num_layers-1){
		for(int j=0; j<node_count[layer]; j++){
			del[layer][j] = a[layer][j] - y[io_count][j];
		}
		//del[layer][0] = 0.0;
	}
	else{
		if(layer>0)
			matrix_mul('d', layer, BACKWARD);
		
		matrix_mul('D', layer, BACKWARD);
	}
}

//Print the values of theta
void print_theta(){

	int limit;

	for(int l=0; l<num_layers-1; l++){
		cout<<endl<<"=============== Layer "<<l<<" ==============="<<endl<<endl;

		if(l==num_layers-2){
			limit = node_count[l+1];
		}
		else{
			limit = node_count[l+1]-1;
		}

		for(int i=0; i<limit; i++){
			for(int j=0; j<node_count[l]; j++){
				cout<< theta[l][i][j]<<"\t";
			}
			cout<<endl;
		}
	}
}

void initialize_DELTA(){
	
	int limit;

	for(int l=0; l<num_layers-1; l++){
		
		if(l==num_layers-2)
			limit = node_count[l+1];
		else
			limit = node_count[l+1]-1;


		for(int i=0; i<limit; i++){
			for(int j=0; j<node_count[l]; j++){
				DELTA[l][i][j] = 0.0;
			}
		}
	}
}

void make_DELTA(){
	DELTA = (double***) malloc((num_layers-1)*sizeof(double**));

	for(int l=0; l<num_layers-1; l++){
		DELTA[l] = (double**) malloc((node_count[l+1]-1)*sizeof(double*));

		for(int i=0; i<node_count[l+1]-1; i++){
			DELTA[l][i] = (double*) calloc(node_count[l],sizeof(double));
		}
	}
}

void initialize_del(){
	del = (double**) malloc((num_layers)*sizeof(double*));

	for(int l=num_layers-1; l>=0; l--){
		del[l] = (double*) calloc(node_count[l],sizeof(double));
	}
}

//Initializes the a-vector
void make_a(){
	a = (double**) malloc((num_layers)*sizeof(double*));

	for(int l=0; l<num_layers; l++){
		a[l] = (double*) calloc(node_count[l],sizeof(double));
	}
}

void initialize_a(){

	for(int l=0; l<num_layers; l++){
		for(int i=0; i<node_count[l]; i++){
			a[l][i]=0.0;
		}
	}
}

void load_input(){

	cout<<"======= Inside load_input ============"<<endl;

	for(int l=0; l<num_layers; l++){
		a[l][0] = 1.0;	//Bias element
	}

	for(int i=1; i<=num_inputs; i++){
		a[0][i] = x[io_count][i-1];
	}


	for(int l=0; l<num_layers; l++){
		for(int i=0; i<node_count[l]; i++)
			cout<<a[l][i]<<" ";
		cout<<endl;
	}
	cout<<endl;

}

//Initializes the z-vector
void initialize_z(){
	
	for(int l=0; l<num_layers; l++){
		z[l][0] = 1.0;	//Bias element
		for(int i=1; i<node_count[l]; i++){
			z[l][i]=0.0;
		}
	}
}

void make_z(){
	z= (double**) malloc((num_layers)*sizeof(double*));

	for(int l=0; l<num_layers; l++){
		z[l] = (double*) calloc(node_count[l],sizeof(double));
	}
}

//Make theta matrix and initialize it to random values
void initialize_theta(){
	theta = (double***) malloc((num_layers-1)*sizeof(double**));
	int limit;

	for(int l=0; l<num_layers-1; l++){
		
		if(l==num_layers-2)
			limit = node_count[l+1];
		else
			limit = node_count[l+1] -1;

		theta[l] = (double**) malloc((limit)*sizeof(double*));

		for(int i=0; i<limit; i++){
			theta[l][i] = (double*) calloc(node_count[l],sizeof(double));
		}
	}

	for(int l=0; l<num_layers-1; l++){

		if(l==num_layers-2)
			limit = node_count[l+1];
		else
			limit = node_count[l+1]-1;

		for(int i=0; i<limit; i++){
			for(int j=0; j<node_count[l]; j++){
				do{
					theta[l][i][j] = RANDOM_FRACTION;
				}while(theta[l][i][j] < 0.1 || theta[l][i][j] > 0.9);
			}
		}
	}
}