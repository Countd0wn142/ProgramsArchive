/*
	+-------------------------------------------------------------------------------+
	|	Author	:	Brady Barnett													|													|
	|	Description: 	Program that inputs a series of tweets and compares them	|
	|					based on Cosine Simularity. The program also removes		|
	|					any stopwords from the tweets provided by an input file		|
	|			 		to reduce redundancy										|
	+-------------------------------------------------------------------------------+
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

double CALC_COS_SIM(vector<string> TESTING, vector<string> TRAINING);
bool IS_REPEAT(vector<string>, string);

int main()
{
		//DECLARE VARIABLES AND INPUT STREAMS
	ifstream TWEET_INPUT;
	ifstream STOPWORDS_INPUT;

	vector<vector<string> > TRAINING_TWEETS;
	vector<vector<string> > TRAINING_TWEETS_RAW;
	vector<vector<string> > TESTING_TWEETS;
	vector<vector<string> > TESTING_TWEETS_RAW;
	vector<string> WORDS;
	vector<string> STOPWORDS;
	string TEMP;	


/*
	+---------------------------------------------------------------+
	|																|
	|	    **SECTION 1: DATA INPUT AND STORAGE**					|
	|																|
	+---------------------------------------------------------------+
*/

		//INPUT
	cout << "Please type the exact* name of stopwords file: ";
	cin >> TEMP;

	STOPWORDS_INPUT.open(TEMP.c_str());
	if(!STOPWORDS_INPUT.is_open())
	{
		cout << "Incorrect filename, exiting program.";
		return 0;
	}

	cout << "Please type the exact* name of the tweet file: ";
	cin >> TEMP;

	TWEET_INPUT.open(TEMP.c_str());
	if(!TWEET_INPUT.is_open())
	{
		cout << "Incorrect filename, exiting program.";
		cout << "\n:: " << TEMP;
		return 0;
	}


		//INPUT ALL STOPWORDS INTO VECTOR	
	while(STOPWORDS_INPUT >> TEMP)
	{
		STOPWORDS.push_back(TEMP);
	}

		//INPUT ALL **PRACTICE** TWEETS INTO INDIVIDUAL WORDS IN SEPERATED SETS AND BREAK ON FIRST WHITESPACE
	while(getline(TWEET_INPUT, TEMP))
	{
			//BREAK WHEN FIRST WHITESPACE APPEARS (SIGNAL THAT PRACTICE TWEETS ARE OVER
		if(TEMP.length() == 0)
			break;

			//SET STRING STREAM TO TEMP AND PARSE WORDS
		istringstream TEMP_SENTENCE(TEMP);
		while(TEMP_SENTENCE >> TEMP)
		{
			if(!IS_REPEAT(WORDS, TEMP))
			{
				WORDS.push_back(TEMP);
			}
		}

		TRAINING_TWEETS.push_back(WORDS);
		WORDS.clear();
	}

		//INPUT ALL **REAL** TWEETS INTO INDIVIDUAL WORDS IN SEPERATED SETS
	while(getline(TWEET_INPUT, TEMP))
	{	
			//IN CASE OF EXTRA WHITESPACE
		if(TEMP.length() == 1)
			continue;

		istringstream TEMP_SENTENCE(TEMP);
		while(TEMP_SENTENCE >> TEMP)
		{
			WORDS.push_back(TEMP);
		}

		TESTING_TWEETS.push_back(WORDS);
		WORDS.clear();
	}

	TESTING_TWEETS_RAW = TESTING_TWEETS;
	TRAINING_TWEETS_RAW = TRAINING_TWEETS;

/*	+---------------------------------------------------------------+
	|																|
i	|	 **SECTION 2: DATA MANIPULATION AND ANALYSIS**				|
	|																|
	+---------------------------------------------------------------+
*/

		//CHANGE ALL LETTERS TO LOWER-CASE IN EVERY **TRAINING** TWEET
	for(int i = 0; i < TRAINING_TWEETS.size(); i++)
	{
			//CYCLE THROUGH EVERY WORD IN EACH SENTENCE
		for(int j = 0; j < TRAINING_TWEETS[i].size(); j++)
		{
				//CYCLE THROUGH EVERY CHARACTER IN EACH WORD
			for(int k = 0; k < TRAINING_TWEETS[i][j].size(); k++)
			{
					//LOWERCASE EACH CHARACTER
				TRAINING_TWEETS[i][j][k] = tolower(TRAINING_TWEETS[i][j][k]);
			}
		}

	}


		//CHANGE ALL LETTERS TO LOWER-CASE IN EVERY **TESTING** TWEET
	for(int i = 0; i < TESTING_TWEETS.size(); i++)
	{
		for(int j = 0; j < TESTING_TWEETS[i].size(); j++)
		{
			for(int k = 0; k < TESTING_TWEETS[i][j].size(); k++)
			{
				TESTING_TWEETS[i][j][k] = tolower(TESTING_TWEETS[i][j][k]);
			}
		}
	
	}


		//ERASE STOPWORDS FROM ALL TWEETS
	for(int i = 0; i < STOPWORDS.size(); i++)
	{

			//LOOK THROUGH TRAINING TWEETS VECTOR
		for(int j = 0; j < TRAINING_TWEETS.size(); j++)
		{
				//LOOK THROUGH WORDS AT POSITION "j" IN TRAINING_TWEETS VECTOR
			for(int k = 0; k < TRAINING_TWEETS[j].size(); k++)
			{
					//CHECK FOR COMPARISON
				if(TRAINING_TWEETS[j][k] == STOPWORDS[i])
				{
						//REMOVE WORD IF COMPARISION IS TRUE
					TRAINING_TWEETS[j].erase(TRAINING_TWEETS[j].begin() + k);
				}
			}

		}

			//LOOK THROUGH TESTING TWEETS VECTOR NEXT
		for(int j = 0; j < TESTING_TWEETS.size(); j++)
		{
			for(int k = 0; k < TESTING_TWEETS[j].size(); k++)
			{
				if(TESTING_TWEETS[j][k] == STOPWORDS[i])
				{
					TESTING_TWEETS[j].erase(TESTING_TWEETS[j].begin() + k);
				}
			}
		}		
	}

/*
	+-----------------------------------------------------------------------+
	|																		|
	|			SECTION 3: DATA OUTPUT										|	
	|																		|
	+-----------------------------------------------------------------------+
*/
	bool SWAPPED;
	vector<vector<double> > COS_SIMS;
	vector<vector<int>    > LABELS;
	vector<int> INT_VECT;
	vector<double> DOUBLE_VECT;
	double double_temp;
	int int_temp;


		//Create COS_SIMS vector and store all data
	for(int i = 0; i < TESTING_TWEETS.size(); i++)
	{
		DOUBLE_VECT.clear();
		INT_VECT.clear();

		for(int j = 0; j < TRAINING_TWEETS.size(); j++)
		{
			DOUBLE_VECT.push_back(CALC_COS_SIM(TESTING_TWEETS[i], TRAINING_TWEETS[j]));
			INT_VECT.push_back(j+1);
		}
		COS_SIMS.push_back(DOUBLE_VECT);
		LABELS.push_back(INT_VECT);
	}
	
		//Bubble Sort
	do
	{
		SWAPPED = false;
		for(int i = 0; i < COS_SIMS.size(); i++)
		{
			for(int j = 0; j < (COS_SIMS[i].size() - 1); j++)
			{
				if(COS_SIMS[i][j] < COS_SIMS[i][j+1])
				{	
					double_temp = COS_SIMS[i][j];
					COS_SIMS[i][j] = COS_SIMS[i][j+1];
					COS_SIMS[i][j+1] = double_temp;
					SWAPPED = true;	

					int_temp = LABELS[i][j];
					LABELS[i][j] = LABELS[i][j+1];
					LABELS[i][j+1] = int_temp;			
				}			
			}
		}
	}while(SWAPPED);

	
		//OUTPUT ALL TRAINING TWEETS
	for(int i = 0; i < TRAINING_TWEETS.size(); i++)
	{
		cout << "T" << i+1 << " ";

		for(int j = 0; j < TRAINING_TWEETS[i].size(); j++)
		{
			cout << TRAINING_TWEETS[i][j] << " ";
		}
		
		cout << endl;
	}

		//CYCLE THROUGH ALL TESTING TWEETS
	for(int i = 0; i < TESTING_TWEETS.size(); i++)
	{
			//SKIPLINE
		cout << endl;
			//OUTPUT CURRENT TESTING TWEET
		for(int j = 0; j < TESTING_TWEETS_RAW[i].size(); j++)
		{
			cout << TESTING_TWEETS_RAW[i][j] << " ";
		}
		cout << endl;
			//OUTPUT ALL COSINE SIMULARITIES
		for(int k = 0; k < 5; k++)
		{
			cout.precision(3);
			cout << COS_SIMS[i][k] << " T" << LABELS[i][k] << endl;
		}

			//ACCOUNTS FOR TIES
		if(COS_SIMS[i].size() > 5)
		{
			if(COS_SIMS[i][4] == COS_SIMS[i][5])
			{	
				cout << COS_SIMS[i][5] << " T" << LABELS[i][5] << endl;
			}
		}
	}

	return 0;
}

bool IS_REPEAT(vector<string> SET, string TARGET)
{

		//SET TARGET TO LOWERCASE
	for(int i = 0; i < TARGET.size(); i++)
	{
		TARGET[i] = tolower(TARGET[i]);
	}

	for(int i = 0; i < SET.size(); i++)
	{
			//SET SET[i] TO LOWERCASE
		for(int k = 0; k < SET[i].size(); k++)
		{
			SET[i][k] = tolower(SET[i][k]);
		}
		
			//IF THE TARGET WORD IS IN THE SET, RETURN TRUE
		if(SET[i] == TARGET)
		{
			return true;
		}
	}
	 //ELSE RETURN FALSE
	return false;
}

double CALC_COS_SIM(vector<string> TESTING, vector<string> TRAINING)
{
	double result = 0;
	vector<int> BIT_VECTOR;

		//INITIALIZE THE RESULT BIT VECTOR TO SIZE OF TESTING
	for(int i = 0; i < TESTING.size(); i++)
	{
		BIT_VECTOR.push_back(0);
	}

		//CREATE BIT VECTOR COMPARING TRAINING TO TESTING
	for(int i = 0; i < TESTING.size(); i++)
	{
		for(int j = 0; j < TRAINING.size(); j++)
		{
			if(TRAINING[j] == TESTING[i])
				BIT_VECTOR[i] = 1;
		}
	}


		//CALCULATE NUMERATOR
	for(int i = 0; i < BIT_VECTOR.size(); i++)
	{
		if(BIT_VECTOR[i] == 1)
			result++;
	}

	result = result/(sqrt(TESTING.size())*sqrt(TRAINING.size()));



	return result;

}
