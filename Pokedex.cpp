/*
	+-------------------------------------------------------------------------------+
	|	Author	:	Brady Barnett													|
	|	Date	:	06/18/2018													|
	|	Description: 	Program that emulates a simple Pokedex from the Pokemon		|
						series. It has functions that insert, update, delete, and 	|
						select pokemon or trainers.									|
	+-------------------------------------------------------------------------------+
*/


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream> 
#include <stdlib.h>

using namespace std;

void INSERT_POKEMON(string, string, string, string, string); 
void INSERT_TRAINER(string, string, string, string, string);
void UPDATE_POKEMON(string, string, string, string, string);
void UPDATE_TRAINER(string, string, string, string, string);
void DELETE_POKEMON(string, string, string, string, string);
void DELETE_TRAINER(string, string, string, string, string);
void SELECT_POKEMON(string, string, string, string, string);
void SELECT_TRAINER(string, string, string, string, string);
void WRITE();

struct TRAINER
{
	string FIRST_NAME;
	string LAST_NAME;
	int AGE;
	string BADGES_HELD;
	string POKEMON_OWNED;
};

struct POKEMON
{
	bool EMPTY;
	int ID;
	string NAME;
	string TYPE;
	string STRONG_TO;
	string WEAK_TO;
};

vector<TRAINER> TRAINERS;
vector<POKEMON> POKEMONS;

int main()
{
		//Declare variables
	string temp, MODIFIER, SELECTED_DB;
	string val1, val2, val3, val4, val5;
	int ID, AGE;



		//INPUT FILE
	ifstream INPUT;
	cout << "Please enter name of input file: ";
	cin >> temp;
	INPUT.open(temp.c_str());

		//Temp pokemon struct
	POKEMON p_temp;
	p_temp.EMPTY = true;

		//Create all pokemon "buckets"
	for(int i = 0; i < 900; i++)
	{
		POKEMONS.push_back(p_temp);
	}

	
		//Extract pokemon database from input file
	INPUT >> temp;
	ifstream POKEMON_INPUT;
	POKEMON_INPUT.open(temp.c_str());
	INPUT >> temp;

		//Extract trainer database from input file
	INPUT >> temp;
	ifstream TRAINER_INPUT;
	TRAINER_INPUT.open(temp.c_str());
	INPUT >> temp;

		//removes empty lines from main input file
	getline(INPUT, temp);
	getline(INPUT, temp);
	
		//removes empty lines from pokemon input file
	getline(POKEMON_INPUT, temp);
	getline(POKEMON_INPUT, temp);

		//Extract all data from pokemon input
	while(getline(POKEMON_INPUT, temp))
	{
		istringstream p_temp(temp);
		
		getline(p_temp, temp, '|');
		ID = atoi(temp.c_str());
		POKEMONS[ID-1].ID = ID;
		POKEMONS[ID-1].EMPTY = false;

		getline(p_temp, temp, '|');
		POKEMONS[ID-1].NAME = temp;

		getline(p_temp, temp, '|');
		POKEMONS[ID-1].TYPE = temp;

		getline(p_temp, temp, '|');
		POKEMONS[ID-1].STRONG_TO = temp;

		getline(p_temp, temp, '|');
		POKEMONS[ID-1].WEAK_TO = temp;
	}

		//removes empty lines from trainer file
	getline(TRAINER_INPUT, temp);
	getline(TRAINER_INPUT, temp);

		//Extract all data from trainer input
	while(getline(TRAINER_INPUT, temp))
	{
		TRAINER TRAINER_TEMP;

		istringstream t_temp(temp);

		getline(t_temp, temp, '|');
		TRAINER_TEMP.FIRST_NAME = temp;
		
		getline(t_temp, temp, '|');
		TRAINER_TEMP.LAST_NAME = temp;

		getline(t_temp, temp, '|');
		AGE = atoi(temp.c_str());
		TRAINER_TEMP.AGE = AGE;

		getline(t_temp, temp, '|');
		TRAINER_TEMP.BADGES_HELD = temp;
	
		t_temp >> TRAINER_TEMP.POKEMON_OWNED;

		TRAINERS.push_back(TRAINER_TEMP);
	}

		//Extract lines from input file
	while(getline(INPUT, temp))
	{	
			//Set the string to a stringstream
		istringstream s_temp(temp);
	
			//Parse until first (, gets the modifier needed
		getline(s_temp, MODIFIER, '(');
			//remove second (
		getline(s_temp, temp, '(');
	
			//Store all function parameters in strings (even the int)
		getline(s_temp, val1, '|');
		getline(s_temp, val2, '|');
		getline(s_temp, val3, '|');
		getline(s_temp, val4, '|');
		getline(s_temp, val5, ')');
		getline(s_temp, temp, ',');
		getline(s_temp, SELECTED_DB, ')');

		if(val1 == "" || val2 == "" || val3 == "" || val4 == "" || val5 == "")
		{
			cout << "Error: one or more parameters are empty." << endl;
		}

			//INSERT
		if(MODIFIER == "INSERT")
		{
	
				//Use Pokemon database
			if(SELECTED_DB == "pokemon")
			{
				INSERT_POKEMON(val1, val2, val3, val4, val5);		
			}
				//Use Trainer database
			else if(SELECTED_DB == "trainers")
			{
				INSERT_TRAINER(val1, val2, val3, val4, val5);
			}
			else
				cout << "Error: not a valid database" << endl;
	
		}
			//UPDATE
		else if(MODIFIER == "UPDATE")
		{
				//Use Pokemon database
			if(SELECTED_DB == "pokemon")
			{
				UPDATE_POKEMON(val1, val2, val3, val4, val5);
			}
				//Use Trainers database
			else if(SELECTED_DB == "trainers")
			{
				UPDATE_TRAINER(val1, val2, val3, val4, val5);
			}
			
		}
			//DELETE
		else if(MODIFIER == "DELETE")
		{
				//Use Pokemon database
			if(SELECTED_DB == "pokemon")
			{
				DELETE_POKEMON(val1, val2, val3, val4, val5);
			}
				//Use Trainers database
			else if(SELECTED_DB == "trainers")
			{
				DELETE_TRAINER(val1, val2, val3, val4, val5);
			}
		}
			//SELECT
		else if(MODIFIER == "SELECT")
		{
				//Use Pokemon database
			if(SELECTED_DB == "pokemon")
			{
				SELECT_POKEMON(val1, val2, val3, val4, val5);
			}
				//Use Trainer database
			else if(SELECTED_DB == "trainers")
			{
				SELECT_TRAINER(val1, val3, val3, val4, val5);
			}
		}
		else if(MODIFIER == "WRITE")
		{
			WRITE();
		}
		else
		{
			cout << "Command not found." << endl;
		}

	}



	return 0;
}

void WRITE()
{
		//temp string
	string user_input;

		//get new pokemon file name
	cout << "Enter file name to write pokemon to(.txt at the end): ";
	cin >> user_input;

		//open up new pokemon file
	ofstream POKEMON_OUTPUT;
	POKEMON_OUTPUT.open(user_input.c_str());

		//get new trainer file name
	cout << "Enter file name to write trainers to(.txt at the end): ";
	cin >> user_input;

		//open up new trainer file
	ofstream TRAINER_OUTPUT;
	TRAINER_OUTPUT.open(user_input.c_str());

		//output pokemon database 
	POKEMON_OUTPUT << "ID\nID,NAME,TYPE,STRONG_TO,WEAK_TO\n";
	for(int i = 0; i < POKEMONS.size(); i++)
	{
		if(!POKEMONS[i].EMPTY)
		{
			POKEMON_OUTPUT  << POKEMONS[i].ID << "|"
					<< POKEMONS[i].NAME << "|"
					<< POKEMONS[i].TYPE << "|"
					<< POKEMONS[i].STRONG_TO << "|"
					<< POKEMONS[i].WEAK_TO << endl;
		}
	}


		//output trainer database
	TRAINER_OUTPUT << "FIRSTNAME,LASTNAME\nFIRSTNAME,LASTNAME,AGE,BADGES_HELD,POKEMON_OWNED\n";
	for(int i = 0; i < TRAINERS.size(); i++)
	{
		TRAINER_OUTPUT 	<< TRAINERS[i].FIRST_NAME << "|"
				<< TRAINERS[i].LAST_NAME << "|"
				<< TRAINERS[i].AGE << "|"
				<< TRAINERS[i].BADGES_HELD << "|"
				<< TRAINERS[i].POKEMON_OWNED << endl;
	}
	
	POKEMON_OUTPUT.close();
	TRAINER_OUTPUT.close();

}


void INSERT_POKEMON(string string_ID, string NAME, string TYPE, string STRONG_TO, string WEAK_TO)
{

		//convert string to int
	int ID = atoi(string_ID.c_str());


	if(POKEMONS[ID-1].EMPTY == false)
	{
		cout << "Error: Database already has an entry for this row. INSERT failed" << endl;
	}
	else
	{
		POKEMONS[ID-1].EMPTY = false;
		POKEMONS[ID-1].ID = ID;
		POKEMONS[ID-1].NAME = NAME;
		POKEMONS[ID-1].TYPE = TYPE;
		POKEMONS[ID-1].STRONG_TO = STRONG_TO;
		POKEMONS[ID-1].WEAK_TO = WEAK_TO;
	}
}

void INSERT_TRAINER(string FIRST_NAME, string LAST_NAME, string string_AGE, string BADGES_OWNED, string POKEMON_OWNED)
{
	int AGE = atoi(string_AGE.c_str());

	TRAINER temp;
	temp.FIRST_NAME = FIRST_NAME;
	temp.LAST_NAME = LAST_NAME;
	temp.AGE = AGE;
	temp.BADGES_HELD = BADGES_OWNED;
	temp.POKEMON_OWNED = POKEMON_OWNED;

	TRAINERS.push_back(temp);
}

void UPDATE_POKEMON(string string_ID, string NAME, string TYPE, string STRONG_TO, string WEAK_TO)
{
	int ID = atoi(string_ID.c_str());

	
	if(POKEMONS[ID-1].EMPTY == false)
	{
		POKEMONS[ID-1].ID = ID;
		POKEMONS[ID-1].NAME = NAME;
		POKEMONS[ID-1].TYPE = TYPE;
		POKEMONS[ID-1].STRONG_TO = STRONG_TO;
		POKEMONS[ID-1].WEAK_TO = WEAK_TO;
	}
	else
		cout << "Error: Database doesn't have an entry to match specified key. UPDATE failed." << endl;
}

void UPDATE_TRAINER(string FIRST_NAME, string LAST_NAME, string string_AGE, string BADGES_OWNED, string POKEMON_OWNED)
{
	int AGE = atoi(string_AGE.c_str());
		
		//Search through all trainers
	for(int i = 0; i < TRAINERS.size(); i++)
	{
			//If the name matches
		if(FIRST_NAME == TRAINERS[i].FIRST_NAME && LAST_NAME == TRAINERS[i].LAST_NAME)
		{
			TRAINERS[i].AGE = AGE;
			TRAINERS[i].BADGES_HELD = BADGES_OWNED;
			TRAINERS[i].POKEMON_OWNED = POKEMON_OWNED;
			return;	
		}
	}
	cout << "ERROR: Database doesn't have an entry to match specified key. UPDATE failed." << endl;
}

void DELETE_POKEMON(string string_ID, string NAME, string TYPE, string STRONG_TO, string WEAK_TO)
{

		//Convert string_ID to integer
	int ID = atoi(string_ID.c_str());

		//Declare bool variables(covered later)
	bool 	var1ANY = false,
		var2ANY = false,
		var3ANY = false,
		var4ANY = false,
		var5ANY = false,

		var1FOUND,
		var2FOUND,
		var3FOUND,
		var4FOUND,
		var5FOUND,

		FOUND = false;

		//Check to see if any of the parameters are "*". If they are, assign varXANY to true
	if(string_ID == "*")
		var1ANY = true;
	if(NAME == "*")
		var2ANY = true;
	if(TYPE == "*")
		var3ANY = true;
	if(STRONG_TO == "*")
		var4ANY = true;
	if(WEAK_TO == "*")
		var5ANY = true;
		

		//Cycle through all pokemon
	for(int i = 0; i < POKEMONS.size(); i++)
	{
			//Reset each found variable
		var1FOUND = false;
		var2FOUND = false;
		var3FOUND = false;
		var4FOUND = false;
		var5FOUND = false;

			//These conditions check to see if ALL parameters are matching(true)
		if(!var1ANY)
		{
			if(POKEMONS[i].ID == ID)
			{
				var1FOUND = true;
			}
		}
		else
		{
			var1FOUND = true;
		}
		
		if(POKEMONS[i].NAME == NAME || var2ANY)
		{
			var2FOUND = true;
		}

		if(POKEMONS[i].TYPE == TYPE || var3ANY)
		{
			var3FOUND = true;
		}

		if(POKEMONS[i].STRONG_TO == STRONG_TO || var4ANY)
		{
			var4FOUND = true;
		}
		
		if(POKEMONS[i].WEAK_TO == WEAK_TO || var5ANY)
		{
			var5FOUND = true;
		}

		if(var1FOUND && var2FOUND && var3FOUND && var4FOUND && var5FOUND)
		{
			FOUND = true;
			POKEMONS[i].EMPTY = true;
		}
	}
	if(!FOUND)
	{
		cout << "Delete request could not find target." << endl;
	}
}

void DELETE_TRAINER(string FIRST_NAME, string LAST_NAME, string string_AGE, string BADGES_OWNED, string POKEMON_OWNED)
{


	bool 	var1ANY = false,
		var2ANY = false,
		var3ANY = false,
		var4ANY = false,
		var5ANY = false, 

		var1FOUND,
		var2FOUND,
		var3FOUND,
		var4FOUND,
		var5FOUND,

		FOUND = false;

	int AGE = atoi(string_AGE.c_str());

	if(FIRST_NAME == "*")
		var1ANY = true;
	if(LAST_NAME == "*")
		var2ANY = true;
	if(string_AGE == "*")
		var3ANY = true;
	if(BADGES_OWNED == "*")
		var4ANY = true;
	if(POKEMON_OWNED == "*")
		var5ANY = true;



	for(int i = 0; i < TRAINERS.size(); i++)
	{
		var1FOUND = false;
		var2FOUND = false;
		var3FOUND = false;
		var4FOUND = false;
		var5FOUND = false;

		if(TRAINERS[i].FIRST_NAME == FIRST_NAME || var1ANY)
		{
			var1FOUND = true;
		}

		if(TRAINERS[i].LAST_NAME == LAST_NAME || var2ANY)
		{
			var2FOUND = true;
		}

		if(TRAINERS[i].AGE == AGE || var3ANY)
		{
			var3FOUND = true;
		}

		if(TRAINERS[i].BADGES_HELD == BADGES_OWNED || var4ANY)
		{
			var4FOUND = true;
		}
		
		if(TRAINERS[i].POKEMON_OWNED == POKEMON_OWNED || var5ANY)
		{
			var5FOUND = true;
		}

		if(var1FOUND && var2FOUND && var3FOUND && var4FOUND && var5FOUND)
		{
			FOUND = true;
			cout << TRAINERS[i].FIRST_NAME << " was sucessfully deleted." << endl;
			TRAINERS.erase(TRAINERS.begin() + i);
		}

	}

	if(!FOUND)
	{
		cout << "Delete request could not find correct target." << endl;
	}

}

void SELECT_POKEMON(string string_ID, string NAME, string TYPE, string STRONG_TO, string WEAK_TO)
{
	bool	var1ANY = false,
		var2ANY = false,
		var3ANY = false,
		var4ANY = false,
		var5ANY = false,

		var1FOUND,
		var2FOUND,
		var3FOUND,
		var4FOUND,
		var5FOUND,

		FOUND = false;

	int ID = atoi(string_ID.c_str());

	if(string_ID == "*")
		var1ANY = true;
	if(NAME == "*")
		var2ANY = true;
	if(TYPE == "*")
		var3ANY = true;
	if(STRONG_TO == "*")
		var4ANY = true;
	if(WEAK_TO == "*")
		var5ANY = true;


	cout << "SELECT SEARCH RESULTS(POKEMON):" << endl;

	for(int i = 0; i < POKEMONS.size(); i++)
	{
		var1FOUND = false;
		var2FOUND = false;
		var3FOUND = false;
		var4FOUND = false;
		var5FOUND = false;

		if(POKEMONS[i].ID == ID || var1ANY)
		{
			var1FOUND = true;
		}
		
		if(POKEMONS[i].NAME == NAME || var2ANY)
		{
			var2FOUND = true;
		}
		
		if(POKEMONS[i].TYPE == TYPE || var3ANY)
		{
			var3FOUND = true;
		}

		if(POKEMONS[i].STRONG_TO == STRONG_TO || var4ANY)
		{
			var4FOUND = true;
		}

		if(POKEMONS[i].WEAK_TO == WEAK_TO || var5ANY)
		{
			var5FOUND = true;
		}

		if(var1FOUND && var2FOUND && var3FOUND && var4FOUND && var5FOUND)
		{
			FOUND = true;

			cout << "ID: " << POKEMONS[i].ID << 
				", NAME: " << POKEMONS[i].NAME << 	
				", TYPE: " << POKEMONS[i].TYPE << 
				", STRONG TO: " << POKEMONS[i].STRONG_TO << 
				", WEAK TO: " << POKEMONS[i].WEAK_TO << endl;
		}
	}

	if(!FOUND)
	{
		cout << "No results found for search..." << endl;
	}

}	


void SELECT_TRAINER(string FIRST_NAME, string LAST_NAME, string string_AGE, string BADGES_OWNED, string POKEMON_OWNED)
{
	bool	var1ANY = false,
		var2ANY = false,
		var3ANY = false,
		var4ANY = false,
		var5ANY = false,

		var1FOUND,
		var2FOUND,
		var3FOUND,
		var4FOUND,
		var5FOUND,

		FOUND = false;

	int AGE = atoi(string_AGE.c_str());

	if(FIRST_NAME == "*")
		var1ANY = true;
	if(LAST_NAME == "*")
		var2ANY = true;
	if(string_AGE == "*")
		var3ANY = true;
	if(BADGES_OWNED == "*")
		var4ANY = true;
	if(POKEMON_OWNED == "*")
		var5ANY = true;


	cout << "SELECT SEARCH RESULTS(TRAINER):" << endl;

	for(int i = 0; i < TRAINERS.size(); i++)
	{
		var1FOUND = false;
		var2FOUND = false;
		var3FOUND = false;
		var4FOUND = false;
		var5FOUND = false;

		if(TRAINERS[i].FIRST_NAME == FIRST_NAME || var1ANY)
		{
			var1FOUND = true;
		}

		if(TRAINERS[i].LAST_NAME == LAST_NAME || var2ANY)
		{
			var2FOUND = true;
		}

		if(TRAINERS[i].AGE == AGE || var3ANY)
		{
			var3FOUND = true;
		}

		if(TRAINERS[i].BADGES_HELD == BADGES_OWNED || var4ANY)
		{
			var4FOUND = true;
		}
		
		if(TRAINERS[i].POKEMON_OWNED == POKEMON_OWNED || var5ANY)
		{
			var5FOUND = true;
		}

		if(var1FOUND && var2FOUND && var3FOUND && var4FOUND && var5FOUND)
		{
			FOUND = true;

			cout << "FIRST NAME: " << TRAINERS[i].FIRST_NAME << 
				", LAST NAME: " << TRAINERS[i].LAST_NAME << 
				", AGE: " << TRAINERS[i].AGE << 
				", BADGES OWNED: " << TRAINERS[i].BADGES_HELD << 
				", POKEMON_OWNED: " << TRAINERS[i].POKEMON_OWNED << endl;
		}
	}
	if(!FOUND)
	{
		cout << "Search found no results..." << endl;
	}
}	
