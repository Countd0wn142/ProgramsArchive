/*
	=-------------------------------------------------------------------------------+
	|	Author:		Brady Barnetti													|
	|	Email:		bradybarnett@my.unt.edu											|	
	|	Date:		9/20/2017														|
	|	Description:	Program that creates a database-like structure				|
	|			to record courses and students. The program also					|
	|			has functions to calculate certain properties of					|
	|			the courses and even import gradebooks.								|
	|																				|
	+-------------------------------------------------------------------------------+

*/


#include <iostream>
#include <fstream>

using namespace std;

	//Global Variable Declorations
int studentcount = 0;
int coursecount = 0;


	//Function Delcorations
int MainMenu();
void AddStudent();
void AddCourse();
void AddStudentToCourse();
void AddGrades();
bool CheckCourse(int s, int c);
void PrintGrades();
void PrintStudents();
void StudentAvg();
void PrintAllCourses();
void PrintAllStudents();
void CourseAvg();
void SaveGrades();
void LoadGrades();

	/*
	Student Structure Containing:
	-Name
	-The course count
	-The courses
	*/
struct student
{
	char name[20];
	int coursecount;
	int courses[3];

} student[100];


	/*
	Course Structure Containing:
	-Course Name
	-All student grades
	*/
struct course
{
	char name[20];
	int studentgrades[100][10]; //The rows indicate the students ID (100 max) while the columns indicate the test grade (10 max)

} course[25];



int main()
{
	int choice;
	char cont = 'y';

	cout << "\t\tAuthor: Brady Barnett\n\t\tEmail: bradybarnett@my.unt.edu\n\t\tDate: 09/20/2017\n\t\tDepartment: CSCE\n\t\tCourse: CSCE 1040\n\t\t";



	/*
		Menu Loop
		Calls the MainMenu() function to obtain the user choice, 
			then passes this info to another function based
			on the switch.
	*/
	while(cont == 'y' || cont == 'Y')
	{

		//Main Menu
		choice = MainMenu();


		switch(choice)
		{
			case 1:
				
				if(coursecount == 24)
				{
					cout << "\nError: The max number of courses has been reached.";
					break;
				}
				else
				{	
					AddCourse();
					break;
				}
			
			case 2: 
				if(studentcount == 100)
				{
					cout << "\nError: The max number of students has been reached.";
					break;
				}
				else
				{
					AddStudent();
					break;
				}
			case 3:
				AddStudentToCourse();
				break;
			case 4:
				AddGrades();
				break;
			case 5:
				PrintGrades();
				break;
			case 6:
				PrintStudents();
				break;
			case 7:
				StudentAvg();
				break;
			case 8:
				PrintAllCourses();
				break;
			case 9:
				PrintAllStudents();
				break;
			case 10:
				CourseAvg();
				break;
			case 11: 
				SaveGrades();
				break;
			case 12:
				LoadGrades();
				break;
			default:
				cont = 'n';
				break;
							
		}


		//Continue Confirmation
	//	cout << "\n\nWould you like to continue? y/n: ";
	//	cin >> cont;
		cout << endl;
			

	}
	
	return 0;
}


//MainMenu Function to obtain users response
int MainMenu()
{
	int userchoice;

	cout << "\nMain Menu\n";
	cout << "1) Add a new course\n";
	cout << "2) Add a new student\n";
	cout << "3) Add a student to a course\n";
	cout << "4) Add grades for a student in a course\n";
	cout << "5) Print a list of all grades for a student in a course\n";	
	cout << "6) Print a list of all students in a course\n";
	cout << "7) Compute the average for a student in a course\n";
	cout << "8) Print a list of all courses\n";
	cout << "9) Print a list of all students\n";
	cout << "10) Compute the average for a course\n";
	cout << "11) Store Grade Book (to a disk file)\n";
	cout << "12) Load Grade Book (from a disk file\n";

	cin >> userchoice;

	return userchoice;

}

//AddStudent function that adds a new student to the student structure.
void AddStudent()
{
	cout << endl << "Please enter the name of the student(without spaces): ";
	cin >> student[++studentcount].name;
	student[studentcount].coursecount = 0;
	for(int i = 0; i < 4; i++)
		student[studentcount].courses[i] = -1;

	cout << endl << student[studentcount].name << " was successfully added to the DB! ID #: " << studentcount << "\nPlease save this number.";
}

//AddCourse Function that adds a new course to the course structure array
void AddCourse()
{
	cout << endl << "Please enter the name of the course(without spaces): ";
	cin >> course[++coursecount].name;
	
	for(int i = 0; i < 100; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			course[coursecount].studentgrades[i][j] = -1;
		}
	}

	cout << "\nCourse was successfully added! " << course[coursecount].name << "'s ID # is: " << coursecount;
}

//AddStudentToCourse function that adds a student to the specified course
void AddStudentToCourse()
{
	int s, c;

	//Input
	cout << "\nPlease enter the student # and the course # that you want to add the student to: ";
	cin >> s >> c;

	//Checks to see if student is already enrolled in 4 classes, then adds them to a course if not.
	if(student[s].coursecount == 3)
		cout << "\nError: This student is already enrolled in the max number of classes. Please try again.";

	else
		student[s].courses[student[s].coursecount++] = c;

	//Updates student in gradebook for specified course
	course[c].studentgrades[s][0] = -2;

	cout << endl << student[s].name << " was successfully added to " << course[c].name << ".";

}

//Function AddGrades that adds grades to a course based on the input. 
void AddGrades()
{
	int s, c, g, count;
	char cont = 'y';

	//Input 
	cout << "\nPlease enter the Student ID and Course ID: ";
	cin >> s >> c;


	//CHecks to see if student ID is in Course ID
	if(CheckCourse(s, c))
	{

		/*
			Finds the position to put the next grade entry
		*/
		for(count = 0; count < 11; )
		{
			if(course[c].studentgrades[s][count] == -1)
				break;
			else
				count++;
		}


		while(cont == 'y' || cont == 'Y')
		{

			
			//Error Handling
			if(count == 11)
			{
				cout << "\nError: The maximum number of grades has been entered for this student.";
				break;
			}
			else
			{
				//Input 
				cout << "\nEnter a grade for the student in this course: ";
				cin >> g;

				//Sets grade in structure
				course[c].studentgrades[s][count] = g;
				
				//Update Current Count
				count++;

				cout << "\nContinue inputing grades? y/n : ";
				cin >> cont;

			}
				
		}
	}
	//Error Handling
	else 
		cout << "\nError: This student is not registered for this course. Please try again.";

}

//PrintGrades function that just obtains the grades for a certain student in a course structure
void PrintGrades()
{
	int s, c;
	
	cout << "\nPlease enter the student ID and course ID seperated by a space: ";
	cin >> s >> c;

	if(CheckCourse(s, c))
	{
		cout << endl << student[s].name << "'s grades for the class are:";
		for(int i = 1; i < 11; i++)
		{
			if(course[c].studentgrades[s][i] >= 0)
				cout << " " << course[c].studentgrades[s][i];
			else
				break;
		}
	}
	else
		cout << "\nError: This student is not registered for this course. Please try again.";

	
}

//Bool function that makes sure a student is enrolled in a class
bool CheckCourse(int s, int c)
{
	if(student[s].courses[0] == c || student[s].courses[1] == c || student[s].courses[2] == c || student[s].courses[3] == c)
		return true;
	else 
		return false;
}

//Prints all students 
void PrintStudents()
{
	int c;

	cout << "\nPlease eter the Course ID: ";
	cin >> c;
	
	cout << "\nThe following students are enrolled in " << course[c].name << ":";
	for(int i = 0; i < 100; i++)
			if(course[c].studentgrades[i][0] == -2)
				cout << " " << student[i].name << ",";
}

//Prints a certain students average
void StudentAvg()
{
	int s, c, count, total = 0;

	cout << "\nPlease enter the Student ID and Course ID seperated by a space: ";
	cin >> s >> c;

	if(CheckCourse(s, c))
	{
		for(int i = 1; i < 11; i++)
		{
			//Make sure there are valid grades
			if(course[c].studentgrades[s][i] == -1)
				break;
			else
			{
				total += course[c].studentgrades[s][i];
				count++;	
			}
		}

		total = total / count;
		cout << "\nThe average for " << student[s].name << " in course '" << course[c].name << "' is: " << total;
	}
	else
		cout << "\nError: This student is not enrolled in this course. Please try again.";


}

//function to print all courses registered
void PrintAllCourses()
{
	cout << "\nHere is a list of all registered courses: ";
	for(int i = 0; i <= coursecount; i++)
		cout << endl << course[i].name;

}

//Function to print all students
void PrintAllStudents()
{
	cout << "\nHere is a list of all registered students: ";
	for(int i = 0; i <= studentcount; i++)
		cout << endl << student[i].name;

}

//function that prints Course average
void CourseAvg()
{
	int c, smallMean, bigMean = 0, bigCount = 0, smallCount;

	cout << "\nPlease enter the Course ID: ";
	cin >> c;

	for(int i = 0; i < 100; i++)
	{
		//Check for registered courses
		if(course[c].studentgrades[i][0] == -2)
		{
			//Intitalize variables
			smallCount = 0, smallMean = 0;

			//Sum up grades
			for(int j = 1; j < 11; j++)
			{
				if(course[c].studentgrades[i][j] == -1)
					break;	
				else
				{
					smallMean += course[c].studentgrades[i][j];
					smallCount++;
				}
			}

			//Cacluate individual average, then add it to the overrall average
			smallMean = smallMean / smallCount;
			bigMean += smallMean;
			bigCount++;
		}

	}
	//Calculate overall average
	bigMean = bigMean / bigCount;

	cout << "\nThe average for " << course[c].name << " is: " << bigMean;

}

//Function that saves grades from a certain course
void SaveGrades()
{
	int c;
	char name[20];
	
	cout << "\nWhat Course ID would you like to save: ";
	cin >> c;

	cout << "\nPlease enter the name of the save file: ";
	cin >> name;
	
	//Create file
	ofstream myfile(name);

	myfile << c << " ";
	myfile << course[c].name;

	for(int i = 0; i < 100; i++)
	{
		for(int j = 0; j < 11; j++)
		{
			myfile << " " << course[c].studentgrades[i][j];

		}
		
	}

	myfile.close();


}

//Function that loads grades from a save file
void LoadGrades()
{
	int oldID;
	char name[20];

	cout << "\nWhat is the name of the Course Gradebook file you want to import? ";
	cin >> name;

	ifstream myfile(name);

	myfile >> oldID;
	myfile >> course[++coursecount].name;

	

	for(int i = 0; i < 100; i++)
	{
		for(int j = 0; j < 11; j++)
		{
			myfile >> course[coursecount].studentgrades[i][j];
		}
	}

	cout << "\nThe old Course ID for this gradebook was: " << oldID << " and has been converted to: " << coursecount;
	cout << "\nThe gradebook was successfully imported!";

	myfile.close();	
}
