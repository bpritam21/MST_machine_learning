#include <set>
#include <utility>
#include <algorithm>    // std::set_difference, std::sort
#include <vector>       // std::vector
//#include "readfromfile.cpp"
//#include "graphs.cpp"


int source_node_ext =0;
int destination_node_ext =0;
int tree_index_ext=0;
int* distTs_ext;
int* Tree_ext;


int nearest_node_ext()
{
	int* nodes_ext;
	nodes_ext = new int [number_of_nodes];
	int dn_ext,sn_ext;
	if(tree_index_ext < number_of_nodes)
	{
		for(int i=0; i< number_of_nodes ;i++)
		{
			nodes_ext[i]=i;
		}
		if(tree_index_ext==0)
		{
			int minimum_distance_ext = node[1].radius;
				
			for (int i=1; i< number_of_nodes; i++)
			{
			
				if (minimum_distance_ext>=node[i].radius)
				{
					minimum_distance_ext = node[i].radius;
					destination_node_ext=i;
					//cout << "is the dest node " << destination_node << "\n" ;
					source_node_ext=0;
					
				}
			}
			cout << source_node_ext << " is the source " << destination_node_ext << " is the dest for source 0\n" ; 
		
		}
			
		else
		{
			int* nodes_not_in_tree_ext;
			nodes_not_in_tree_ext = new int [number_of_nodes-tree_index_ext-1];
			nodes_ext = new int [number_of_nodes];
			for(int i=0;i<number_of_nodes;i++)
				nodes_ext[i]=i;
			std::vector<int> v(number_of_nodes-tree_index_ext-1);                    
			std::vector<int>::iterator it;
			std::sort (nodes_ext,nodes_ext+number_of_nodes);     //  5 10 15 20 25
			std::sort (Tree_ext,Tree_ext+tree_index_ext+1);   // 10 20 30 40 50
			it=std::set_difference (nodes_ext, nodes_ext+number_of_nodes, Tree_ext, Tree_ext+tree_index_ext+1, v.begin());
			v.resize(it-v.begin());                 
			std::copy(v.begin(), v.end(), nodes_not_in_tree_ext);
			for (int i=0;i<number_of_nodes-tree_index_ext-1; i++)
				cout << nodes_not_in_tree_ext[i] << " are the nodes not in tree \n";

			int Distances_in_the_tree_ext[tree_index_ext+1][number_of_nodes-tree_index_ext-1];
		
			int min_ext = 10000000;
			for (int i=0; i<=tree_index_ext; i++ ) 
			{
				int temp1 = Tree_ext[i];				
				for( int k=0;k<number_of_nodes-tree_index_ext-1; k++)
				{
					
					int temp2 = nodes_not_in_tree_ext[k];
					Distances_in_the_tree_ext[i][k] = abs((node[temp1].xcoordinate)-(node[temp2].xcoordinate)) + abs((node[temp1].ycoordinate)-(node[temp2].ycoordinate));
					if(Distances_in_the_tree_ext[i][k] < min_ext)
					{
						sn_ext=temp1;
						dn_ext=temp2;
						min_ext=Distances_in_the_tree_ext[i][k];
						
			
					}			
				}
				
							
			}
			cout << min_ext << " is the min dist \n";
			source_node_ext=sn_ext;
			destination_node_ext=dn_ext;	
			//cout << " source_node is " << source_node << "\n";	
			//cout << " dest node is " << destination_node << "\n";
		}
}
return 0;
}
	

int Construct_Tree_H2()
{

	nearest_node_ext();
	cout << source_node_ext << " is the source node\n";
	cout << destination_node_ext << " is the destination node \n";
	int distx_y = abs((node[destination_node_ext].xcoordinate)-(node[source_node_ext].xcoordinate)) + abs((node[destination_node_ext].ycoordinate)-(node[source_node_ext].ycoordinate)); 
	// choosing the the nearest node in the tree wrt destination
	int mindist_ext = distx_y;
	
	
}







int H_extensions()
{
	distTs = new int [number_of_nodes];
	gh = new Graph(number_of_nodes);
	Tree = new int [number_of_nodes];

        if(number_of_nodes)
	{
		readfromfile(node); 
		
	}
	while(tree_index!=number_of_nodes-1)
	{
		if(H2==1)
		{
			Construct_Tree_H2();
		}	
	}
	cout << "wirelength is" << wirelength << "\n";
	
	//gh->printGraph();

return 0;
} 

