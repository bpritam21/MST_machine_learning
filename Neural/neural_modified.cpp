#include "neural.h"

int main(int argc, char *argv[]){

	srand((long int)time(NULL));
	
	if(argc < 4){
		cout<<"Invalid number of parameters"<<endl;
		cout<<"The format is: ./neural [path-to-model] [path-to-training-set] [path-to-testing-set]"<<endl;
		exit(1);
	}

	read_model(argv);

	last_layer = num_layers-1;
	theta_layers = num_layers-1;
	last_theta_layer = theta_layers-1;
	
	make_system();

	int choice = TRAIN;

	do{

		cout<<"Enter a valid choice: (1-TRAIN or 2-TEST or 3-EXIT)"<<endl;
		fflush(stdin);
		scanf("%d",&choice);

		if(choice == TRAIN || choice > EXIT){
			read_dataset(argv,TRAIN);
			int iter=0, max_iter = choice-EXIT;

			while((choice > EXIT && iter < max_iter)){

				initialize_DELTA();
				cout<<endl<<max_iter<<endl;
				print_theta();

				for(io_count=0; io_count<set; io_count++){
					initialize_del();
					load_input();
					forward_prop();
					//cout<<endl<<"====================== set: "<<io_count<<" ================================="<<endl;
					//print_z();
					//cout<<endl;
					//print_a();
					partial_cost();
					backward_prop();
				}
				calculate_D();
				update_theta();
				update_learning_rate();
				iter++;
			}

			cout<<endl<<"After training"<<endl;
			// cout<<endl;
			print_theta();
			cout<<endl;
			print_z();
			cout<<endl;
			print_a();
			cout<<endl;
			print_del();
			cout<<endl;
			print_DELTA();
			cout<<endl;
			print_D();
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

	}while(choice !=EXIT);				

	// cout<<endl;
	// print_theta();
	// // cout<<endl;
	// // print_z();
	// cout<<endl;
	// print_a();
	// cout<<endl;
	// print_del();
	// cout<<endl;
	// print_DELTA();
	// cout<<endl;
	// print_D();

	// print_x();
	// print_y();

	return 0;
}

void initialize_a(){

	for(int l=0; l<num_layers; l++){
		for(int i=0; i<node_count[l]; i++){
			a[l][i]=0.0;
		}
	}
}

void print_result(){
	for(int i=1; i<node_count[0]; i++){
		cout<<a[0][i]<<" ";
	}

	for(int i=0; i<node_count[last_layer]; i++){
		cout<<" "<<a[num_layers-1][i];
	}
	cout<<"   Cost: "<<cost;
	cout<<endl;
}

void update_learning_rate(){
	
	cost = (double) cost/set;
	
	double regulation_factor=0.0;
	int limit;

	for(int l=0; l<theta_layers; l++){

		if(l==last_theta_layer){
			limit = node_count[l+1];
		}
		else{
			limit = node_count[l+1]-1;
		}
		
		for(int i=0; i<limit; i++){
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
	
	int limit;

	for(int l=0; l<theta_layers; l++){

		if(l==last_theta_layer){
			limit = node_count[l+1];
		}
		else{
			limit = node_count[l+1]-1;
		}
		
		for(int i=0; i<limit; i++){
			for(int j=0; j<node_count[l]; j++){
				
				theta[l][i][j] = theta[l][i][j] - learning_rate*D[l][i][j];
			}
		}
	}
}

void calculate_D(){
	int limit;

	for(int l=0; l<theta_layers; l++){
		
		if(l==last_theta_layer){
			limit = node_count[l+1];
		}
		else{
			limit = node_count[l+1]-1;
		}

		for(int i=0; i<limit; i++){
			for(int j=0; j<node_count[l]; j++){
				
				D[l][i][j] = (double) DELTA[l][i][j]/set;
					if(j>0)
						D[l][i][j] += (double) lambda*theta[l][i][j];
			}
		}
	}
}

void backward_prop(){
	for(int l=last_layer; l>=0; l--){
		calculate_del(l);
	}
}

void calculate_del(int layer){
	
	if(layer == last_layer){
		for(int j=0; j<node_count[layer]; j++){
			del[layer][j] = a[layer][j] - y[io_count][j];
		}
	}
	else{
		if(layer>0)
			matrix_mul('d', layer, BACKWARD);
		
		matrix_mul('D', layer, BACKWARD);
	}
}

void partial_cost(){

	for(int i =0; i < num_outputs; i++){
		cost = (double) cost + (-y[io_count][i]*log(a[num_layers-1][i]) - (1 - y[io_count][i])*log(1 - a[num_layers-1][i]));
	}
}

void forward_prop(){

	for(int l=0; l<last_layer; l++){
		matrix_mul('z',l,FORWARD);
		calculate_a(l+1);
	}
}

void calculate_a(int layer){

	for(int i=1; i<node_count[layer];i++){
		a[layer][i] = signum(z[layer][i]);
	}

	if(layer == last_layer)
		a[layer][0] = signum(z[layer][0]);
	else
		a[layer][0] = 1.0;

}

double signum(double input){
	double x = -input;
	double out;

	x = 1 + exp(x);
	out = 1/x;
	return out;
}

void matrix_mul(char matrix_sel, int layer, bool flow){
	int i, limit;

	if(flow){

		if(layer == last_layer-1){
			limit = 0;
			i=0;
		}
		else{
			limit = 1;
			i=0;
			z[layer+1][0] = 1.0;
		}


		while(limit < node_count[layer+1]){
			z[layer+1][limit] =0.0;
			for(int j=0; j<node_count[layer]; j++){

				z[layer+1][limit] += theta[layer][i][j]*a[layer][j];
			}
			i++;
			limit++;
		}

	}else{
		
		if(matrix_sel == 'd'){
			
			double* temp = (double*) calloc(node_count[layer],sizeof(double));

			int j, limit;

			for(int i=0;i<node_count[layer]; i++){
				
				if(layer == last_layer-1){
					limit = 0;
					j = 0;

				}else{
					limit = 1;
					j=0;
				}

				del[layer][i]=0.0;

				while(limit<node_count[layer+1]){
					
					del[layer][i] += theta[layer][j][i]*del[layer+1][limit];

					cout<<del[layer][i]<<" = "<<theta[layer][j][i]<<" * "<<del[layer+1][limit]<<endl;
					j++;
					limit++;
				}

			}
			
			for(int i=0; i<node_count[layer];i++)
				temp[i] = a[layer][i]*(1.0 - a[layer][i]);

			for(int i=0;i<node_count[layer];i++)
				del[layer][i] *= temp[i];
		}

		if(matrix_sel == 'D'){

			int i, limit;

			if(layer == last_layer-1){
				limit = 0;
				i = 0;

			}else{
				limit = 1;
				i=0;
			}

			while(limit<node_count[layer+1]){
				for(int j=0; j<node_count[layer]; j++){
					
					DELTA[layer][i][j] += del[layer+1][limit]*a[layer][j];
				}
				i++;
				limit++;
			}
		}
	}
}

void initialize_DELTA(){
	
	int limit;

	for(int l=0; l<theta_layers; l++){
		
		if(l==last_theta_layer)
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

void load_input(){

	for(int l=0; l<num_layers; l++){
		a[l][0] = 1.0;	//Bias element
	}

	for(int i=1; i<=num_inputs; i++){
		a[0][i] = x[io_count][i-1];
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

void print_y(){
	for(int i=0; i<set; i++){
		cout<<"================ y set-"<<i<<"=================="<<endl;
		for(int j=0; j<num_outputs; j++)
			cout<<y[i][j]<<" ";
		cout<<endl;
	}
}

void print_x(){
	
	for(int i=0; i<set; i++){
		cout<<"================ x set-"<<i<<"=================="<<endl;
		for(int j=0; j<num_inputs; j++)
			cout<<x[i][j]<<" ";
		cout<<endl;
	}
}

void make_system(){

	initialize_theta();
	make_z();
	initialize_z();
	make_a();
	make_DELTA();
	initialize_D();
	old_cost = 99999999.9999;
}

void print_D(){

	int limit;

	for(int l=0; l<theta_layers; l++){
		cout<<endl<<"=============== D Layer "<<l<<" ==============="<<endl<<endl;

		if(l==last_theta_layer){
			limit = node_count[l+1];
		}
		else{
			limit = node_count[l+1]-1;
		}

		for(int i=0; i<limit; i++){
			for(int j=0; j<node_count[l]; j++){
				cout<< D[l][i][j]<<"\t";
			}
			cout<<endl;
		}
	}
}

void initialize_D(){
	D = (double***) malloc(theta_layers*sizeof(double**));
	int limit;

	for(int l=0; l<theta_layers; l++){

		if(l==last_theta_layer)
			limit = node_count[l+1];
		else
			limit = node_count[l+1] -1;
		
		D[l] = (double**) malloc(limit*sizeof(double*));

		for(int i=0; i<limit; i++){
			D[l][i] = (double*) calloc(node_count[l],sizeof(double));
		}
	}
}

void print_DELTA(){

	int limit;

	for(int l=0; l<theta_layers; l++){
		cout<<endl<<"=============== DELTA Layer "<<l<<" ==============="<<endl<<endl;

		if(l==last_theta_layer){
			limit = node_count[l+1];
		}
		else{
			limit = node_count[l+1]-1;
		}

		for(int i=0; i<limit; i++){
			for(int j=0; j<node_count[l]; j++){
				cout<< DELTA[l][i][j]<<"\t";
			}
			cout<<endl;
		}
	}
}

void make_DELTA(){
	DELTA = (double***) malloc((theta_layers)*sizeof(double**));
	int limit;

	for(int l=0; l<theta_layers; l++){

		if(l==last_theta_layer)
			limit = node_count[l+1];
		else
			limit = node_count[l+1] - 1;

		DELTA[l] = (double**) malloc(limit*sizeof(double*));

		for(int i=0; i<limit; i++){
			DELTA[l][i] = (double*) calloc(node_count[l],sizeof(double));
		}
	}
}

void print_del(){
	for(int l=last_layer; l>=0; l--){
		cout<<"================ del Layer "<<l<<"================="<<endl;
		for(int i=0; i<node_count[l]; i++)
			cout<< del[l][i]<< " ";
		cout << endl;
	}
}

void initialize_del(){
	del = (double**) malloc((num_layers)*sizeof(double*));

	for(int l=last_layer; l>=0; l--){
		del[l] = (double*) calloc(node_count[l],sizeof(double));
	}
}

void print_a(){
	for(int l=0; l<num_layers; l++){
		cout<<"================ a Layer "<<l<<"================="<<endl;
		for(int i=0; i<node_count[l]; i++)
			cout<< a[l][i]<< " ";
		cout << endl;
	}
}

//Initializes the a-vector
void make_a(){
	a = (double**) malloc((num_layers)*sizeof(double*));

	for(int l=0; l<num_layers; l++){
		a[l] = (double*) calloc(node_count[l],sizeof(double));
	}
}

//Initializes the z-vector
void initialize_z(){
	
	for(int l=0; l<last_layer; l++){
		z[l][0] = 1.0;	//Bias element
	}
}

//Prints the z values
void print_z(){
	for(int l=0; l<num_layers; l++){
		cout<<"================ z Layer "<<l<<"================="<<endl;
		for(int i=0; i<node_count[l]; i++)
			cout<< z[l][i]<< " ";
		cout << endl;
	}
}

//Makes the z matrix
void make_z(){
	z= (double**) malloc((num_layers)*sizeof(double*));

	for(int l=0; l<num_layers; l++){
		z[l] = (double*) calloc(node_count[l],sizeof(double));
	}
}

//Prints the theta values
void print_theta(){

	int limit;

	for(int l=0; l<theta_layers; l++){
		cout<<endl<<"=============== theta Layer "<<l<<" ==============="<<endl<<endl;

		if(l==last_theta_layer){
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

//Make theta matrix and initialize it to random values
void initialize_theta(){
	theta = (double***) malloc((theta_layers)*sizeof(double**));
	int limit;

	for(int l=0; l<theta_layers; l++){
		
		if(l== last_theta_layer)
			limit = node_count[l+1];
		else
			limit = node_count[l+1] -1;

		theta[l] = (double**) malloc((limit)*sizeof(double*));

		for(int i=0; i<limit; i++){
			theta[l][i] = (double*) calloc(node_count[l],sizeof(double));
		}
	}

	for(int l=0; l<theta_layers; l++){

		if(l==last_theta_layer)
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

		cout<< "Set: "<<set<<endl;
		cout<< "Num_Layers: "<<num_layers<<endl;
		cout<< "Lambda: "<<lambda<<endl;
		cout<< "Learning Rate: "<<learning_rate<<endl;
		cout<< "Number of Inputs: "<<num_inputs<<endl;
		cout<< "Number of Outputs: "<<num_outputs<<endl;

		node_count = (int*) calloc(num_layers,sizeof(int));

		for(int i=0; i<num_layers; i++){
			myfile >> node_count[i];
		}
	}
}