#include "main.h"
#include <cstdlib> 
using namespace std;
 
/*
 * Adjacency List Node
 */ 
 struct AdjListNode
{
    int dest;
    int distance;
    struct AdjListNode* next;
};
 
/*
 * Adjacency List
 */  
struct AdjList
{
    struct AdjListNode *head;
};
 
/*
 * Class Graph
 */
class Graph
{
    public:
        int V;
        struct AdjList* array;
        Graph(int V)
        {
            this->V = V;
            array = new AdjList [V];
            for (int i = 0; i < V; ++i)
                array[i].head = NULL;
			
        }
        /*
         * Creating New Adjacency List Node
         */ 
        AdjListNode* newAdjListNode(int dest,int dist)
        {
            AdjListNode* newNode = new AdjListNode;
	    newNode->distance=dist;
            newNode->dest = dest;
            newNode->next = NULL;
			
            return newNode;
        }


     /*
         * Adding Edge to Graph
         */ 
        void addEdge(int src, int dest,int distance)
        {
            AdjListNode* newNode = newAdjListNode( dest , distance);
            newNode->next = array[src].head;
            array[src].head = newNode;
            newNode = newAdjListNode( src, distance);
            newNode->next = array[dest].head;
            array[dest].head = newNode;
       }
	
	
	void DFST ( int parent_Node,bool visited[] )
	{
		int v;
		
		for (v=0; v < V; v++)
		{
			AdjListNode* i = array[v].head;
			visited[parent_Node]=true;

			
			//cout << parent_Node << "->";
			parent_Node = i->dest;
			if(i!=NULL)
			{
				if(!visited[parent_Node])
				{
					
					cout << i->dest<< "->";
					//cout << i << "->";
					i=i->next;			
					DFST(parent_Node,visited);
					
					
					//cout << "here \n";
				} 
				else
				{
					//cout << " error here \n";
						}
			}
			
		
		}
	
	}
	void DFS(int parent_node)
	{
		 bool *visited = new bool[V];
    		 for(int i = 0; i < V; i++)
       		 visited[i] = false;
		 DFST(parent_node,visited);
	}


 /*
         * Print the graph
         */ 
        void printGraph()
        {
            int v;
	    for (v = 0; v < V; ++v)
            {
		
		AdjListNode* disp = array[v].head;
                cout<<"\n Adjacency list of vertex "<< v ;
                while (disp)
                {
                    cout<<"-> "<< disp->dest;
                    disp = disp->next;
					
                }
                cout<<endl;
            }
        }
};
 

/*
 * Main
 */ 
/* int main()
{
    Graph gh(5);
    gh.addEdge(0, 1);
    gh.addEdge(0, 4);
    gh.addEdge(1, 2);
    gh.addEdge(1, 3);
    gh.addEdge(1, 4);
    gh.addEdge(2, 3);
    gh.addEdge(3, 4);
 
    // print the adjacency list representation of the above graph
    gh.printGraph();
 
    return 0;
} */
