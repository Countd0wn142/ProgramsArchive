/*
	+-------------------------------------------------------------------------------+
	|	Author	:	Brady Barnett													|
	|	Date	:	06/18/2018														|
	|	Description: 	Program that checks if two words can be connected through	|
	|					a series of words with only one letter different in each 	|
	|					word given an imported dictionary.							|
	|					ex. cord - core - care - dare
	+-------------------------------------------------------------------------------+
*/






#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

bool CHECK_IF_EDGE(string, string);
bool IS_VISITED(vector<int>, int);

int main()
{
	
	bool FOUND = false;

	string TEMP;
	string INPUT1;
	string INPUT2;
	bool CONTINUE;

	vector<string> ALL_WORDS;
	vector<string> REFINED_WORDS;
	vector<int> TEMP_GRAPH;
	vector<vector<int> > GRAPH;


	cout << "Please input dictionary: ";
	cin >> TEMP;


	ifstream INPUT;
	INPUT.open(TEMP.c_str());


		//INPUT ALL WORDS INTO WORD VECTOR
	while(INPUT >> TEMP)
	{
		ALL_WORDS.push_back(TEMP);
	}

	do
	{
		REFINED_WORDS.clear();
		GRAPH.clear();

		cout << "Please input first word: ";
		cin >> TEMP;
		INPUT1 = TEMP;

		cout << "Please input second word: ";
		cin >> TEMP;
		INPUT2 = TEMP;

		if(INPUT1.size() != INPUT2.size())
		{
			cout << "Error: Words are not of same length. Aborting Process." << endl;
			break;
		}


		//REFINE ALL WORDS TO SAME LENGTH
		for(int i = 0; i < ALL_WORDS.size(); i++)
		{
			if(ALL_WORDS[i].size() == INPUT1.size())
			{
				REFINED_WORDS.push_back(ALL_WORDS[i]);
			}
		}
	
	
	
	
				//CREATE GRAPH OF SIZE NxN (WHERE N = ALL_WORDS.size())
		for(int i = 0; i < REFINED_WORDS.size(); i++)
		{	
			TEMP_GRAPH.clear();	
	
			for(int j = 0; j < REFINED_WORDS.size(); j++)
			{
				
					//CHECK TO SEE IF AT SAME WORD FIRST, OR IF STRINGS ARE DIFFERENT SIZES
					if(REFINED_WORDS[i] == REFINED_WORDS[j])
				{
						TEMP_GRAPH.push_back(0);
				}
					//CHECK TO SEE IF ITS A VALID EDGE
				else if(CHECK_IF_EDGE(REFINED_WORDS[i], REFINED_WORDS[j]))
				{
					TEMP_GRAPH.push_back(1);
				}
					//IF NOT, RETURN ZERO
				else
				{
					TEMP_GRAPH.push_back(0);
				}
				
			}
				//INSERT ROW INTO GRAPH
			GRAPH.push_back(TEMP_GRAPH);
		}
	
		vector<int> VISITED;
		vector<int> QUEUE;
		vector<int> PATH;
		vector<int> PARENTS;
		int COUNT = 0;
		int STARTING_NODE;
		int ENDING_NODE;
	
		PARENTS.clear();
		VISITED.clear();
		QUEUE.clear();
		PATH.clear();

		FOUND = false;

		for(int i = 0; i < REFINED_WORDS.size(); i++)
		{
			PARENTS.push_back(-1);
		}
	
	
	
			//FIND START POSITION
		for(int i = 0; i < REFINED_WORDS.size(); i++)
		{
			if(REFINED_WORDS[i] == INPUT1)
			{
				QUEUE.push_back(i);
				VISITED.push_back(i);
				STARTING_NODE = i;
				break;
			}
	
				//ERROR HANDLING HERE
		}
	
	
		while(!FOUND && QUEUE.size() > 0)
		{
			
			for(int i = 0; i < GRAPH.size(); i++)
			{
					//CHECK TO SEE IF IT IS AN EDGE	
				if(GRAPH[QUEUE[0]][i] == 1)
				{
						//CHECK IF VISITED ALREADY
					if(!IS_VISITED(VISITED, i))
					{
						PARENTS[i] = QUEUE[0];
						QUEUE.push_back(i);
						VISITED.push_back(i);	
	
						if(REFINED_WORDS[i] == INPUT2)
						{
							ENDING_NODE = i;
							PATH.push_back(i);
							FOUND = true;
							break;
						}
					}
					
				}
	
			}
			QUEUE.erase(QUEUE.begin());
		}
	
		
		if(FOUND)	
		{
			bool FINISHED = false;
			int LEAF = ENDING_NODE;
			int PARENT_NODE;
	
			while(!FINISHED)
			{
				PARENT_NODE = PARENTS[LEAF];
		
	
				PATH.push_back(PARENT_NODE);
				LEAF = PARENT_NODE;
			
				if(PARENT_NODE == STARTING_NODE)
				{
					FINISHED = true;
				}
			}
	
	
			cout << "PATH IS: ";
			for(int i = PATH.size()-1; i >= 0; i--)
			{	
				cout << REFINED_WORDS[PATH[i]] << " ";
			}
	
			cout << endl;	
	
	
		}
		else
		{
			cout << "Path not available" << endl;
		}

	cout << "Continue? y/n: ";
	cin >> TEMP;

	if(TEMP == "n" || TEMP == "N" || TEMP == "no")
		CONTINUE = false;
	else if(TEMP == "y" || TEMP == "Y" || TEMP == "yes")
		CONTINUE = true;
	else
	{
		cout << "Incorrect option, terminating program." << endl;
		CONTINUE = false;
	}
	

	}while(CONTINUE);

	return 0;
}


bool CHECK_IF_EDGE(string A, string B)
{
	/*
		This function checks to see if the words are edges by parsing through every individual character in
		both strings and comparing them. If an individual comparison is true, then the count will increase
		by one. By the end of the loop, if the count is equal to [A.size() - 1], then that means only one 
		character comparison failed, and the nodes indeed share an edge.
	*/


		//SET OUR COUNTER TO ZERO
	int COUNT = 0;

	if(A.size() != B.size())
		return false;


	for(int i = 0; i < A.size(); i++)
	{
		if(A[i] != B[i])
		{
			COUNT++;
		}
		if(COUNT > 1)
			return false;
	}

		return true;


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


