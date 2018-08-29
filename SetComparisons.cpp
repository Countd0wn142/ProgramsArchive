/*
	+-------------------------------------------------------------------------------+
	|	Author	:	Brady Barnett													|													|
	|	Description: 	Program that inputs a series of items in a set and compares	|
	|					them to another set via bit vector. 						|
	+-------------------------------------------------------------------------------+
*/






#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

vector <string> data;
vector <string> UniversalSet;
vector <string> P1;
vector <string> P2;

bool Search(string target, int person);


int main()
{
	string str, fileName;
	int count = 0;
	bool flag1 = false;		//bools that identify whether P1 or P2 already has a record of items
	bool flag2 = false;

	ifstream input;			//open file

	cout <<  "\nPlease enter file name to import: ";
	cin >> fileName;
	input.open(fileName.c_str());


	getline(input, str); 		//obtain the universal set
	data.push_back(str);		//put non-seperated set in the data vector

	while(getline(input, str))	//keep obtaining lines until end of file
	{

		if(str[1] == '1')	//check the 2nd character of line (the persons number) to identify which set it belongs to.
		{
			str.erase (0, 3); //Deletes the "P1:"

			if(!flag1)	//If flag1 is false, then add new entry		
			{
				data.push_back(str);
				flag1 = true;
			}
					
			else		//else add to existing entry
			{ 
				data[1].erase(data[data.size()-1].length()-1, 1);	//remove return spacing
				data[1].append(",");
				data[1].append(str);
			}
		}

		else if(str[1] == '2')
		{
			str.erase(0, 3); //Deletes the "P2:"

			if(!flag2)
			{
				data.push_back(str);
				flag2 = true;
			}
			else
			{
				data[2].erase(data[data.size()-1].length()-1, 1);
				data[2].append(",");
				data[2].append(str);
			}

		}
	}

	for(int i = 0; i < data.size(); i++)		//loop that splits all the items in the data vector into individual peices within unique vectors
	{
		stringstream temp;
		temp << data[i];

		while(getline(temp, str, ','))
		{

			if(i == 0)
			{
				UniversalSet.push_back(str);
			}
			else if(i == 1)
			{
				P1.push_back(str);
			}
			else if(i == 2)
			{
				P2.push_back(str);
			}
		}

	}

	vector <int> UniBitVector (UniversalSet.size());	//create bitvectors with the same size as the universal set
	vector <int> P1BitVector  (UniversalSet.size());
	vector <int> P2BitVector  (UniversalSet.size());


	UniversalSet[UniversalSet.size()-1].erase(UniversalSet[UniversalSet.size()-1].length()-1, 1);

	P1[P1.size()-1].erase(P1[P1.size()-1].length()-1, 1);
	P2[P2.size()-1].erase(P2[P2.size()-1].length()-1, 1);
	

	for(int i = 0; i < P1BitVector.size(); i++)		//assign P1BitVector a 1 or 0 depending on if the search item was found
	{
		if(Search(UniversalSet[i], 1))
		{
			P1BitVector[i] = 1;
		}
		else
		{
			P1BitVector[i] = 0;
		}
	}

	for(int i = 0; i < P2BitVector.size(); i++)		//assign P2BitVector (at i) a 1 or 0 depending on if the search item was found in main set
	{
		if(Search(UniversalSet[i], 2))
		{
			P2BitVector[i] = 1;
		}
		else
		{
			P2BitVector[i] = 0;
		}
	}

	for(int i = 0; i < UniBitVector.size(); i++)		//Compares P1BitVector and P2BitVector for the things in common (universally)
	{
		if(P1BitVector[i] == 1 || P2BitVector[i] == 1)
		{
			UniBitVector[i] = 1;
		}
		else
		{
			UniBitVector[i]  = 0;
		}
	}	
	
	cout << "\nFormat is Bit Vectors. A 1 represents that an item is present, and a 0 represents not having an item.";
	cout << "\nPositioning of the bit vector is relative to the order given from the full list as follows.";
	cout << "\nFull list of items: ";
	for(int i = 0; i < UniversalSet.size(); i++)
		cout << UniversalSet[i] << " ";
	cout << "\nPerson 1's belongings: ";
	for(int i = 0; i < P1BitVector.size(); i++)
		cout << P1BitVector[i] << " ";
	cout << "\nPerson 2's belongings: ";
	for(int i = 0; i < P2BitVector.size(); i++)
		cout << P2BitVector[i] << " ";
	cout << "\nCombined belongings:   ";
	for(int i = 0; i < UniBitVector.size();i++)
		cout << UniBitVector[i] << " ";


	cout << "\nDuplicate Items:       ";
	for(int i = 0; i < P1BitVector.size(); i++)
	{
		if(P1BitVector[i] == 1 && P2BitVector[i] == 1)
			cout << "1 ";
		else
			cout << "0 ";
	}


	cout << "\n\nEnd of program.\n";


	
	return 0;
}

bool Search(string target, int person)			//search function that returns true if found or false if not
{
	if(person == 1)
	{
		for(int i = 0; i < P1.size(); i++)
		{
			if(P1[i] == target)
			{	
				return true;
			}
		}
		
	}
	else if(person == 2)
	{
		for(int i = 0; i < P2.size(); i++)
		{
			if(P2[i] == target)
			{
				return true;
			}
		}
	}

	return false;
}
