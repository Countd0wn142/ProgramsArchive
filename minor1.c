/*
	+-----------------------------------------------------------------------+
	|	Author		:	Brady Barnett				|
	|	Date		:	9/6/2018				|
	|	Description	:	Program that converts normal ascii	|
	|				characters (up to 127) to decimal,	|
	|				hex, octal, and binary bases.		|
	+-----------------------------------------------------------------------+
*/


#include <stdio.h>
#include <stdlib.h>

	//Function decloration
void convert_to_base(int number, int base);

int main()
{
	char input;
	int ascii;

		//Input
	printf("Enter an ASCII character: ");
	scanf("%c", &input);

	ascii = input;	
	
		//Print out for user
	printf("dec -- %d\n", ascii);
	printf("hex -- ");
		//call conversion function
	convert_to_base(ascii, 16);
	printf("\noct -- ");
	convert_to_base(ascii, 8);
	printf("\nbin -- 0");
	convert_to_base(ascii, 2);
	printf("\n");
		
	
	return 0;
}

void convert_to_base(int number, int base)
{
		//create a remainder from the start
	int remainder = number % base;

		//if the number ever hits zero, the function returns
	if(number == 0)
	{
		return;
	}
		//recursively keep calling until a number of zero is reached
	convert_to_base(number/base, base);

		//checks if remainder is under 10, this is mainly for Hexadecimal
	if(remainder < 10)
	{
		printf("%d", remainder);
	}
	else
	{
		printf("%c", remainder-10+'A');
	}
}
