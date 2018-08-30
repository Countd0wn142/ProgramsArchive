/*
	+-------------------------------------------------------------------------------+
	|	Author	:	Brady Barnett													|
	|	Description: 	Program that calculates the propegation time based on a 	|
	|					Depth First Search and a Breadth First search. The 			|
	|					application uses these searches to traverse a node graph	|
	|					while calculating each individual propegation time.			|
	+-------------------------------------------------------------------------------+
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

static int SENTINEL_VALUE = -1;
vector< vector<int> > data;

void OUTPUT_GRAPH(int);
int CALCULATE_PROP(vector<int>);
bool IS_VISITED_2(vector<vector<int> >, int);
bool IS_VISITED(vector<int>, int);
void DFS(int, int);
void BFS(int, int);

int main()
{
		//initialize variables
	string TEXT;
	int n;
	int TEMP_PROP;
	int TEMP_ROW;
	int TEMP_COL;
	int TEMP_START;
	int TEMP_END;

		//open input stream
	ifstream input;

	cout << "Please enter the name of the input file: ";
	cin >> TEXT;
	input.open(TEXT.c_str());

		//get first input to determine the size of the 2D matrix (n x n)
	input >> n;


		//initialize the vector of vector<int>'s (2D Vector of type <int>)
	for(int i = 0; i < n; i++)
	{
		vector<int> TEMP_DATA;		

		for(int j = 0; j < n; j++)
		{
			TEMP_DATA.push_back(0);
		}

	data.push_back(TEMP_DATA);
	}


			//apply node transversals from input file	
	while(input >> TEMP_ROW)
	{
			//exit if sentinel value is found
		if(TEMP_ROW == SENTINEL_VALUE)
			break;

			//get the col its being sent to
		input >> TEMP_COL;
		input >> TEMP_PROP;

		data[TEMP_ROW-1][TEMP_COL-1] = TEMP_PROP;
		data[TEMP_COL-1][TEMP_ROW-1] = TEMP_PROP;
	}


	OUTPUT_GRAPH(n);

	while(input >> TEMP_START)
	{
		input >> TEMP_END;
		
		cout << "\nDFS(" << TEMP_START << "->" << TEMP_END << ") :";		
		DFS(TEMP_START-1, TEMP_END-1);

		cout << "\nBFS(" << TEMP_START << "->" << TEMP_END << ") :";
		BFS(TEMP_START-1, TEMP_END-1);

	}
	

	return 0;
}

void DFS(int START, int END)
{
	vector<int> STACK;
	vector<int> VISITED;
	bool FOUND = false;
	

			//start the stack with the first node given
	STACK.push_back(START);
	VISITED.push_back(START);
	while(STACK.size() > 0 && FOUND == false)
	{


				//go through the data 2D vector to determine adjacent nodes
		for(int i = 0; i < data.size(); i++)
		{	

					//if node has a propegation time > 0, it is connected
			if(data[STACK[STACK.size()-1]][i] > 0)
			{


				if(i == END)
				{
					
					STACK.push_back(i);
					FOUND = true;
					break;
				}
	

						//check to see if node has been visited yet
				if(!IS_VISITED(VISITED, i))
				{

					STACK.push_back(i);
					VISITED.push_back(i);
					break;
				}
			}

				//remove last entry if no nodes are available
			if(i == (data.size()-1))
				STACK.erase(STACK.begin() + (STACK.size()-1));

		}
	}

	if(FOUND)
	{
		cout << " FOUND!\nPath is: ";
		for(int i = 0; i < STACK.size(); i++)
		{
			cout << (STACK[i]+1) << " ";
		}
		cout << "\nPropagation total: " << CALCULATE_PROP(STACK);
		cout << endl;
	}
	else
		cout << " NOT FOUND!\nNo Path Available" << endl;
}

bool IS_VISITED(vector<int> VISITED, int TARGET)
{
	for(int i = 0; i < VISITED.size(); i++)
	{
		if(VISITED[i] == TARGET)
			return true;
	}

	return false;
}

bool IS_VISITED_2(vector<vector<int> > VISITED, int TARGET)
{
	for(int i = 0; i < VISITED.size(); i++)
	{
		for(int j = 0; j < VISITED[i].size(); j++)
		{
			if(VISITED[i][j] == TARGET)
				return true;
		}
		
	} 


	return false;
}

void BFS(int START, int END)
{

	bool FOUND = false;
	vector<int> QUEUE;
	vector<vector<int> > VISITED;
	vector<int> TEMP_VISITED;
	vector<int> R_PATH;

		//put the start into the que
	QUEUE.push_back(START);
	TEMP_VISITED.push_back(START);
	VISITED.push_back(TEMP_VISITED);
	while(QUEUE.size() > 0 && FOUND == false)
	{
		TEMP_VISITED.clear();

			//Start searching through 2D vector for adjacent nodes
		for(int i = 0; i < data.size(); i++)
		{

				//check to see if nodes are adjacent by looking at propegation times
			if(data[QUEUE[0]][i] > 0)
			{

					//if target is found add to visited list and break out of cycle
				if(i == END)
				{
					TEMP_VISITED.push_back(i);
					FOUND = true;
					break;
				}
				


					//Check to see if node has already been visited
				if(!IS_VISITED_2(VISITED, i))
				{
					TEMP_VISITED.push_back(i);
					QUEUE.push_back(i);
				}

			}
		}

		VISITED.push_back(TEMP_VISITED);
		QUEUE.erase(QUEUE.begin());
	}

	if(FOUND)
	{
		int TEMP_NODE;
		int TIER = (VISITED.size()-1);

		R_PATH.push_back(VISITED[VISITED.size()-1][VISITED[VISITED.size()-1].size()-1]);
		TEMP_NODE = R_PATH[0];

		while(TIER > 0)
		{
			for(int i = 0; i < data.size(); i++)
			{
				if(data[TEMP_NODE][i] > 0)
				{		
					if(IS_VISITED(VISITED[TIER], i))
					{
						R_PATH.push_back(i);
						TEMP_NODE = i;
						break;
					}
				}			

			}
			TIER -= 1;
		}

		R_PATH.push_back(VISITED[0][0]);

		cout << " FOUND!\nPath is: ";
		for(int i = R_PATH.size(); i > 0; i--)
		{
			cout << R_PATH[i-1]+1 << " ";
		}
		cout << "\nPropagation total: " << CALCULATE_PROP(R_PATH);
		cout << endl;
	}
	else
	{
		cout << " NOT FOUND!\nNo Path Available" << endl;

	}

}

int CALCULATE_PROP(vector<int> NODES)
{
	int sum = 0;

	while(NODES.size() > 1)
	{
		sum += data[NODES[NODES.size()-1]][NODES[NODES.size()-2]];
		NODES.erase(NODES.begin() + (NODES.size()-1));
	}

	return sum;

}


		//void function to return the traversal graph
void OUTPUT_GRAPH(int n)
{

	cout << "\n\t";
	for(int i = 0; i < n; i++)
		cout << right << setw(4) << (i+1);


	for(int i = 0; i < n; i++)
	{
		cout << "\n    " << right << setw(4) << (i+1);

		for(int j = 0; j < n; j++)
		{
			cout << right << setw(4) << data[i][j];
		}
	}
	cout << endl;
}
