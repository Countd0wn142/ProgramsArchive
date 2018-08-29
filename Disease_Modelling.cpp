/*
	+-------------------------------------------------------------------------------+
	|	Author	:	Brady Barnett													|
	|	Description: 	Program that models a disease given certain input paramters	|
	|					such as infection rate, population, interaction averages,	|
	|					and more. This application is helpful to predict outbreaks  |
	|					and project the spreading of a given disease				|
	+-------------------------------------------------------------------------------+
*/



#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;


struct People						//Create struct for individual persons
{								//includes contact count and their current state
	int ContactCount;				
	string SIRstate;
	int InfectedPeriod;				 
};

vector<int> FinalS;		//all global variables
vector<int> FinalR;
vector<int> Ends;
vector<int> Peaks;
vector<int> UnfinishedPool;
vector<People> Population;
double InfProb;
int InfPeriod;
bool InfectedLeft = true;

void SetPopulation(int, int, int, int);		//prototypes
void Interaction(int, int);
void OvernightUpdate(int);
void SimulateDay(int);

int main()
{

	string str;					//Declare our variables
	int temp;
	int simCount = 0;
	vector <int> values;	
	ifstream input;	


	cout << "Please enter the input file name: ";	//Get name of input file to open
	cin >> str;

	input.open(str.c_str());			//Open given file


	while(input.ignore(256, '=') >> temp)		//Extract data from file into a vector
	{						//Position	Type
		values.push_back(temp);			// [0]		 # of Susceptible (starting)
	}						// [1]		 # of Infectious (starting)
							// [2]		 # of Recovered (starting)
							// [3]		 % of Vaccinated (starting)
							// [4] 		 Contact Rate
							// [5]		 Transmission Probabilty
							// [6]		 Infectious Period
							// [7]		 Simulations to run 


	InfProb = values[5];
	InfPeriod = values[6];

	cout << "\nDISEASE MODEL PARAMETERS:";		//read off inputs
	cout << "\n# Susceptible = " << values[0];
	cout << "\n# of Infectious = " << values[1];
	cout << "\n# of Recovered = " << values[2];
	cout << "\n% of Vaccinated = " << values[3];
	cout << "\nContact Rate = " << values[4];
	cout << "\nTransmission Probability = " << values[5];
	cout << "\nInfectious Period = " << values[6];
	cout << "\nSimulations to run = " << values[7] << endl;


	/*Actual Simulation Starts Here*/

	while(simCount < values[7])		//overall simulation loop
	{

		InfectedLeft = true;				//reset all values
		Population.clear();
		SetPopulation(values[0], values[1], values[2], values[3]);

		int count = 1;

		Ends.push_back(0);			//increment the avg value vectors
		Peaks.push_back(0);
		FinalS.push_back(0);
		FinalR.push_back(0);

		while(InfectedLeft)		//individual simulation loop
		{

			InfectedLeft = false;
	
			SimulateDay(values[4]);
	
			OvernightUpdate(count);
	
			count++;
		}

		Ends[Ends.size()-1] = count;
		simCount++;
	}			

	int sum = 0;
	
	for(int i = 0; i < Ends.size(); i++)		//averages calculations
	{
		sum += Ends[i];
	}
	
	cout << "\nAverage End Day: " << (sum/Ends.size());
	
	sum = 0;
	
	for(int i = 0; i < Peaks.size(); i++)
	{
		sum += Peaks[i];
	}

	cout << "\nAverage Peak Day: " << (sum/Peaks.size());

	sum = 0;

	for(int i = 0; i < FinalS.size(); i++)
	{
		sum += FinalS[i];
	}

	cout << "\nAverage Number of Susceptible Agents Left: " << (sum/FinalS.size());

	sum = 0;

	for(int i = 0; i < FinalR.size(); i++)
	{
		sum += FinalR[i];
	}

	cout << "\nAverage Number of Recovered Agents: " << (sum/FinalR.size()+1) << endl << endl;

	return 0;
}



void SimulateDay(int dailyContacts)
{

	srand(time(NULL));
	int random;
	UnfinishedPool.clear();
	UnfinishedPool.push_back(0);

	for(int i = 0; i < Population.size()-1; i++)	//set all unfinished contacts in one vector
	{
		UnfinishedPool.push_back(i+1);
	}


	while (UnfinishedPool.size() > 1)
	{
		while(Population[UnfinishedPool[0]].ContactCount < dailyContacts)
		{

				//pick random number 1 - UnfinishedPool.size()

			int check = UnfinishedPool.size() - 1;
			if(check == 0)
				check++;

			random = ((rand() % check) + 1);

			Interaction(UnfinishedPool[0], UnfinishedPool[random]);

				//checks the random unfinished pool for completion of daily contacts
			if(Population[UnfinishedPool[random]].ContactCount == dailyContacts)
				UnfinishedPool.erase(UnfinishedPool.begin() + random);
		}
			//erase finished pool
		UnfinishedPool.erase(UnfinishedPool.begin());
	}  
}

void Interaction(int person1, int person2)
{	

	int luck;

	if(Population[person1].SIRstate == "I" && Population[person2].SIRstate == "S")
	{
		luck = rand() % 100;
		if(luck < InfProb)
		{
			Population[person2].SIRstate = "C";
		}
	}
	else if(Population[person1].SIRstate == "S" && Population[person2].SIRstate == "I")
	{
		luck = rand() % 100;
		if(luck < InfProb)
		{
			Population[person1].SIRstate = "C";
		}
	}

	Population[person1].ContactCount += 1;
	Population[person2].ContactCount += 1;
}

void OvernightUpdate(int dayCount)
{
	int count = 0;
	int Scount = 0;
	int Rcount = 0;

	for(int i = 0; i < Population.size(); i++)		//checks all overnight values
	{
		if(Population[i].SIRstate == "C")
		{
			Population[i].SIRstate = "I";
		}
		else if(Population[i].SIRstate == "I" && Population[i].InfectedPeriod < InfPeriod)
		{
			InfectedLeft = true;
			Population[i].InfectedPeriod += 1;
		}
		else if(Population[i].SIRstate == "I" && Population[i].InfectedPeriod == InfPeriod)
			Population[i].SIRstate = "R";
		else if(Population[i].SIRstate == "R")
			Rcount++;
		else if(Population[i].SIRstate == "S")
			Scount++;
	}
	
	for(int i = 0; i < Population.size(); i++)
	{
		if(Population[i].SIRstate == "I")
			count++;
		Population[i].ContactCount = 0;
	}

	  //checks to see if current infected total is higher than the last
	if(Peaks[Peaks.size() - 1] < count)
		Peaks[Peaks.size() - 1] = dayCount;

	FinalS[FinalS.size()-1] = Scount;
	FinalR[FinalR.size()-1] = Rcount;

}


void SetPopulation(int s, int inf, int r, int v)
{
	int count = 0;					//count keeps track of current vector position

	v = ((v * .01) * s);				//converts v to decimal, then multiplies by total population
	s = s - v;					

	int total = s + inf + r + v;

	People person;
	person.ContactCount = 0;
	person.InfectedPeriod = 1;

	for(int i = 0; i < total; i++)			//Create full population as default people
	{
		Population.push_back(person);
	}
	
/*	for(int i = 0; i < s; i++)
	{
		Population[count].SIRstate = "S";
		count++;
	} */

	for(int i = 0; i < inf; i++)
	{
		Population[count].SIRstate = "I";
		count++;
	}
	
	for(int i = 0; i < r; i++)
	{
		Population[count].SIRstate = "R";
		count++;
	}

	for(int i = 0; i < v; i++)
	{
		Population[count].SIRstate = "V";
		count++;
	}
	
	for(int i = 0; i < s; i++)
	{
		Population[count].SIRstate = "S";
		count++;
	}

}
