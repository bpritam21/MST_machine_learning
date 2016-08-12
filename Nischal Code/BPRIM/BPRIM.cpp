#include <set>
#include <utility>
#include <algorithm>    // std::set_difference, std::sort
#include <vector>       // std::vector
#include "readfromfile.cpp"
#include "BPRIM.h"
#include "graphs.cpp"


int* distTs;
int* Tree;
int tree_index=0;
int tree_index_alias;
Graph* gh;
int next_node=0;
int destination_node=0;
int source_node=0;
int node_in_tree;
struct Node_tracking
{
	std::vector<int> nodetosourcevector;
};

Node_tracking *treespecifictonode;

int Addanedge(int s1, int d1 ,int dis)
{
		gh->addEdge(s1, d1, dis);	 //add an edge between x1 and rest 
		treespecifictonode[d1].nodetosourcevector =  treespecifictonode[s1].nodetosourcevector;
		treespecifictonode[d1].nodetosourcevector.push_back(s1);
		distTs[d1] = distTs[s1] + ((abs(node[s1].xcoordinate - node[d1].xcoordinate)) + (abs(node[s1].ycoordinate - node[d1].ycoordinate)));
		wirelength+=((abs(node[s1].xcoordinate - node[d1].xcoordinate)) + (abs(node[s1].ycoordinate - node[d1].ycoordinate))) ;
		tree_index++;
		Tree[tree_index] = d1; 
		cout << d1 << "is added to the tree \n";
return 0;
}

int appropriate_edge (int x1, int destination_node)
{
	if(x1 >=0 && x1< number_of_nodes)
	{ 
	    if(H1==1 && H2!=1)
	    {	
		int x11;
		int loop_count = treespecifictonode[x1].nodetosourcevector.size()-1;
		cout << loop_count << "  loop count is\n";
		int distx1_y_ext_H1[loop_count];
		int min_dist_in_tree = 1000000000;
		int position = 0;
		int node_in_loop;
		if(loop_count!=0)
		{
			for (int i = 0 ; i <=loop_count ; i++)
			{
				node_in_loop = treespecifictonode[x1].nodetosourcevector[loop_count-i];
				//cout << node_in_loop << " s the node buh buh \n";
				distx1_y_ext_H1[i] = abs((node[destination_node].xcoordinate)-(node[node_in_loop].xcoordinate)) + abs((node[destination_node].ycoordinate)-(node[node_in_loop].ycoordinate));
					if(min_dist_in_tree > distx1_y_ext_H1[i])
					{
						min_dist_in_tree = distx1_y_ext_H1[i];	
						position = loop_count-i;			
					}
			
			}
			cout << min_dist_in_tree << " is the min dist \n";
			cout << treespecifictonode[x1].nodetosourcevector[position] << " is the source node \n";
			x11 = treespecifictonode[x1].nodetosourcevector[position];
		}
		else
		{
			x11=x1;
		}
		Addanedge (x11,destination_node,min_dist_in_tree);
		return 1;
			
	    }

	else if(H2==1 && H1!=1)
	{
		int distxT_y[tree_index];
		int min_distxT_y;
		int position = 0; 
		//distxT_y[0]= abs((node[destination_node].xcoordinate)-(node[x1].xcoordinate)) + abs((node[destination_node].ycoordinate)-(node[x1].ycoordinate));
		min_distxT_y = 100000000;
		
		for(int i=0; i<=tree_index; i++)
		{
   		   if (Tree[i]!=source_node)
		   {	
			distxT_y[i] = abs((node[destination_node].xcoordinate)-(node[Tree[i]].xcoordinate)) + abs((node[destination_node].ycoordinate)-(node[Tree[i]].ycoordinate));
		
			if(min_distxT_y > distxT_y[i])
			{
				min_distxT_y = distxT_y[i];
				position = i;
			}
		   }
		}
		cout << position << "is the position of node in the tree for H2 \n";
		cout << Tree[position] << " is the node in the tree to be added \n";
		cout << min_distxT_y << "  is the min distance \n";
		Addanedge (Tree[position], destination_node, min_distxT_y);
		return 1;

	}



	

	else
	{
		cout << "error \n" << x1 << "is the source node\n";
	
		int distx1_y = abs((node[destination_node].xcoordinate)-(node[x1].xcoordinate)) + abs((node[destination_node].ycoordinate)-(node[x1].ycoordinate)); 
	
			cout << distx1_y << "      is the distance \n";
			cout << distTs[x1] << "  is the distance of the node from tree\n";
			cout << maximum_radius << "is the max radius \n";
			if (distx1_y + distTs[x1] <= maximum_radius)
			{
				Addanedge(x1, destination_node, distx1_y);
				return 1;
			}
			else
			{
				return 0;
			}
		} 
	}
return 100;	
	
}


int nearest_node()
{
	int* nodes;
	nodes = new int [number_of_nodes];
	int dn=0;
	int sn=0;
	if(tree_index < number_of_nodes)
	{
		for(int i=0; i< number_of_nodes ;i++)
		{
			nodes[i]=i;
		}
		if(tree_index==0)
		{
			int minimum_distance = node[1].radius;
				
			for (int i=1; i< number_of_nodes; i++)
			{
			
				if (minimum_distance>=node[i].radius)
				{
					minimum_distance = node[i].radius;
					destination_node=i;
					//cout << "is the dest node " << destination_node << "\n" ;
					source_node=0;
					
				}
			}
			cout << source_node << " is the source " << destination_node << " is the dest for 0\n" ; 
		
		}
			
		else
		{
			int* nodes_not_in_tree;
			nodes_not_in_tree = new int [number_of_nodes-tree_index-1];
			nodes = new int [number_of_nodes];
			for(int i=0;i<number_of_nodes;i++)
				nodes[i]=i;
			std::vector<int> v(number_of_nodes-tree_index-1);                    
			std::vector<int>::iterator it;
			std::sort (nodes,nodes+number_of_nodes);     //  5 10 15 20 25
			std::sort (Tree,Tree+tree_index+1);   // 10 20 30 40 50
			it=std::set_difference (nodes, nodes+number_of_nodes, Tree, Tree+tree_index+1, v.begin());
			v.resize(it-v.begin());                 
			std::copy(v.begin(), v.end(), nodes_not_in_tree);
			for (int i=0;i<number_of_nodes-tree_index-1; i++)
				cout << nodes_not_in_tree[i] << " are the nodes not in tree \n";

			int Distances_in_the_tree[tree_index+1][number_of_nodes-tree_index-1];
		
			int min = 10000000;
			for (int i=0; i<=tree_index; i++ ) 
			{
				int temp1 = Tree[i];				
				for( int k=0;k<number_of_nodes-tree_index-1; k++)
				{
					
					int temp2 = nodes_not_in_tree[k];
					Distances_in_the_tree[i][k] = abs((node[temp1].xcoordinate)-(node[temp2].xcoordinate)) + abs((node[temp1].ycoordinate)-(node[temp2].ycoordinate));
					if(Distances_in_the_tree[i][k] < min)
					{
						
						if(Distances_in_the_tree[i][k]!= min)
						{
							sn=temp1;
							dn=temp2;
							min=Distances_in_the_tree[i][k];
							cout << dn << " dn chnages here\n";
							cout << min << " is the min \n";
						
						}
						
			
					}			
				}
				
							
			}
			cout << min << " is the min dist \n";
			source_node=sn;
			destination_node=dn;	
			cout << " source_node is " << source_node << "\n";	
			cout << " dest node is " << destination_node << "\n";
		}
}
return 0;
}
	



int Construct_Tree  ()
{	
	distTs[0]=0;
	Tree[0]=0;
	
	if(tree_index < number_of_nodes)
	{
		cout << tree_index << " is the tree index, not yet failed\n";
		if(node_in_tree < number_of_nodes-1)
		{
	
	//find the closest node in the tree//
			nearest_node();
			cout << "  destination_node is " << destination_node << "\n";
			int next_node = destination_node;
			int distx_y = abs((node[destination_node].xcoordinate)-(node[source_node].xcoordinate)) + abs((node[destination_node].ycoordinate)-(node[source_node].ycoordinate)); 
	 		cout << distTs[source_node] << " is the  source node dist in the tre \n" << source_node << " is the source node \n";
			if ((distTs[source_node] + distx_y) <= radius_bound)
			{
			
				tree_index++;
				cout << "dest node is " << destination_node << " dist is " << distx_y << "\n";
				cout << source_node << "  is the source \n";
	
				gh->addEdge(destination_node, source_node, distx_y);
				//new trial
			
				//std::vector<int> theconnectingtree = treespecifictonode[destination_node].nodetosourcevector;
				if(source_node==0)
				{
					treespecifictonode[destination_node].nodetosourcevector.push_back(source_node);
				}
				else
				{
					treespecifictonode[destination_node].nodetosourcevector =  treespecifictonode[source_node].nodetosourcevector;
					treespecifictonode[destination_node].nodetosourcevector.push_back(source_node);
				}				
		
				
				size_t last = (treespecifictonode[destination_node].nodetosourcevector.size()) - 1;
				cout << last << " the last item is \n";
				int temp = ((abs(node[destination_node].xcoordinate - node[source_node].xcoordinate)) + (abs(node[destination_node].ycoordinate - node[source_node].ycoordinate)));
				distTs[next_node] = distTs[source_node]+ temp ;
				cout << distTs[next_node] << "is the distance of the new node in the tree added \n" ;
				wirelength+=((abs(node[destination_node].xcoordinate - node[source_node].xcoordinate)) + (abs(node[destination_node].ycoordinate - node[source_node].ycoordinate))) ;
				Tree[tree_index]= destination_node;
				cout << Tree[tree_index] << "is the node added\n" << " in the tree and its source is  " << source_node << "\n"; 
			
			
			}		
		
			else
			{	
				int result;
				if(H1==1 && H2!=1)
				{
					result = appropriate_edge(source_node, destination_node); 
				}
			
				else if (H2==1 && H1!=1)
				{
				
					result = appropriate_edge(source_node, destination_node);  
			
				}
				 		
			 	//tree_index_alias = tree_index;
				//new code
				else
			   	{
			
					cout << source_node << "is the source node \n";
					size_t tree_index_alias = (treespecifictonode[source_node].nodetosourcevector.size()) - 1;
					cout << " index alias is " << tree_index_alias << " \n";
				 
					if(tree_index_alias >=0 )
					{
					
						result = appropriate_edge(treespecifictonode[source_node].nodetosourcevector[tree_index_alias], destination_node);  
						cout << "  here \n" ;
						while(result==0)
						{
							result = appropriate_edge(treespecifictonode[source_node].nodetosourcevector[tree_index_alias], destination_node);
							tree_index_alias--;
						}
					}
				}
			}	
		}
	}  		
return 0;
}	
int BPRIM()
{
        distTs = new int [number_of_nodes];
	gh = new Graph(number_of_nodes);
	Tree = new int [number_of_nodes];
	treespecifictonode = new Node_tracking [number_of_nodes];
	tree_index=0;
	if(number_of_nodes)
	{
		readfromfile(node); 
		
	}
	while(tree_index!=number_of_nodes-1)
	{
		Construct_Tree();
	}
	cout << "wirelength is" << wirelength << "\n";
	
	gh->printGraph();

return 0;
} 

