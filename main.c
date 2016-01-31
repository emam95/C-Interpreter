#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "lib.h"
#include "symboltable.h"

int main()
{
	//start statment

	initHash(symbolTable, tableSize);
	while(1)
	{
		int error = 0;
		char input[100];
		char commands[100];
		printf("* ");
		getInput(input);
		separateCommand(input, commands);
		handleCommand(commands);
	}

	//end statment

	return 0;
}
