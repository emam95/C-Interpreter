#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "symboltable.h"
#include "lib.h"

int tableSize = 100;
listOfVar symbolTable[100];

unsigned long long getHash(char* str)
{
	unsigned long long hash = 17;
	int i;
	for(i = 0; i<strlen(str) ; i++)
	{
		hash = ((int)str[i] + (31 * hash)) % (unsigned long long)1e10;
	}
	return hash;
}

int getIndex(char* str, int tableSize)
{
	unsigned long long x = getHash(str);
	int y = x % tableSize;
	return y;
}

void initlistOfVar(listOfVar* l)
{
	l->head = NULL;
	l->tail = NULL;
}

void initHash(listOfVar hashTable[], int tableSize)
{
	int i;
	for(i = 0; i < tableSize; i++)
	{
		initlistOfVar(&hashTable[i]);
	}
}

var* newVar(char* Name, Decimal data,int tableSize)
{
	var* v = (var*)malloc(sizeof(var));
	strcpy(v->Name,Name);
	v->value = data;
	v->key = getIndex(Name, tableSize);
	v->next = NULL;
	return v;
}

void insertHashTable(char* str,listOfVar hashTable[],int tableSize,Decimal value)
{
	var* v = newVar(str, value, tableSize);
	var* tail = hashTable[v->key].tail;
	hashTable[v->key].tail = v;
	if(tail != NULL)
		tail->next = v;
	else
		hashTable[v->key].head = v;
}

var* searchHash(listOfVar hashTable[], int tableSize, char* str)
{
	int index = getIndex(str, tableSize);
	var* v = hashTable[index].head;
	while(v != NULL)
	{
		if(strcmp(v->Name, str) == 0)
			return v;
		v = v->next;
	}
	return NULL;
}

void printHash(listOfVar hashTable[],int tableSize)
{
	printf("SymbolTable\n");
	int i;
	var* v;
	for(i = 0; i < tableSize; i++)
	{
		v = hashTable[i].head;
		while(v != NULL)
		{
			printf("%s\t%.2f\n",v->Name,v->value.value);
			v = v->next;
		}
	}
}

void initDecimal(Decimal *d)
{
	d->integer = 0;
	d->decimal = 0;
	d->decimalPlaces = 0;
	d->value = 0;
	d->isDecimal = 0;
}

void createVar(char* exp)
{
	int i , j = 0,flag = 0;
	Decimal value;
	initDecimal(&value);
	char name[50];
	for(i = 0; i < strlen(exp); i++)
	{
		if(exp[i] == ' ' || exp[i] == '\n')
			continue;
		if(isalpha(exp[i]))
		{
			if(flag)
			{
				printf("error: wrong variable name\n");
				return;
			}
			name[j] = exp[i];
			j++;
			i++;
			int k = i+1;
			while(isalpha(exp[i]))
			{
				name[j] = exp[i];
				j++;
				i++;
			}
			name[j] = '\0';
			flag = 1;
		}
		if((exp[i]>= '0' && exp[i]<= '9') || exp[i] == '.')
		{
			if(exp[i]>= '0' && exp[i]<= '9')
			{
				value.integer = exp[i] - '0';
				int j = i+1;
				while(exp[j]>= '0' && exp[j]<= '9')
				{
					value.integer *= 10;
					value.integer += (exp[j] - '0');
					j++;
					i++;
				}
				i++;
			}
			if(exp[i] == '.')
			{
				int k = i+1;
				while(exp[k]>= '0' && exp[k] <= '9')
				{
					value.decimal *= 10;
					value.decimal += (exp[k] - '0');
					k++;
					i++;
					value.decimalPlaces++;
				}
			}
			value.value = value.integer + (value.decimal / pow(10,value.decimalPlaces));
		}
	}
	var* v = searchHash(symbolTable, tableSize, name);
	if(v == NULL)
		insertHashTable(name, symbolTable, tableSize, value);
	else
	{
		printf("variable already defined\n");
		return;		
	}	
}


void handleExpression(char* exp)
{
	int i , j = 0, k ,flag = 0;
	char name[50];
	for(i = 0; exp[i] != '='; i++)
	{
		if(exp[i] == ' ' || exp[i] == '\n')
			continue;
		if(isalpha(exp[i]))
		{
			if(flag)
			{
				printf("error: wrong variable name\n");
				return;
			}
			name[j] = exp[i];
			j++;
			k = i+1;
			while(isalpha(exp[k]))
			{
				name[j] = exp[k];
				j++;
				i++;
			}
			name[j] = '\0';
			flag = 1;
		}
	}
	i++;
	var* v = searchHash(symbolTable, tableSize, name);
	if(v == NULL)
	{
		printf("variable %s not defined\n",name);
		return;
	}
	
	char infixExp[100];
	int z = 0,y;
	for(; i < strlen(exp); i++)
	{
		if(exp[i] == ' ' || exp[i] == '\n')
			continue;
		if((exp[i]>= '0' && exp[i]<= '9')||exp[i] == '.')
		{
			infixExp[z] = exp[i];
			z++;
		}
		else if(isalpha(exp[i]))
		{
			j = 0;
			name[j] = exp[i];
			j++;
			i++;
			k = i+1;
			while(isalpha(exp[i]))
			{
				name[j] = exp[i];
				j++;
				i++;
			}
			i--;
			name[j] = '\0';
			var* temp = searchHash(symbolTable, tableSize, name);
			if(temp == NULL)
			{
				printf("variable %s not defined\n",name);
				return;
			}
			int numTemp = temp->value.integer;
			int numtemp = 0;
			
			while(numTemp > 0)
			{
				numtemp = numTemp  % 10;
				infixExp[z] = numtemp + '0';
				z++;
				numTemp /= 10;
			}
			infixExp[z] = '.';
			z++;
			numTemp = temp->value.decimal;
			numtemp = 0;

			for(y = 0; y < temp->value.decimalPlaces ; y++)
			{
				numtemp = numTemp  % 10;
				infixExp[z] = numtemp + '0';
				z++;
				numTemp /= 10;
			}
		}
		else
		{
			infixExp[z] = exp[i];
			z++;
		}
			
		
	}
	infixExp[z] = '\0';
	
	double x;
	int error = 0;
	char postfixExp[100];
	convertInfix(infixExp, postfixExp, &error);
	if(error == 0)
	{
		x = evaluatePostFix(postfixExp, &error);
		v->value.value = x;
		v->value.decimalPlaces = countDecimal(x);
		v->value.integer = (int)x;
		double floatTemp = x - v->value.integer;
		for(int s = 0;s < v->value.decimalPlaces;s++)
			floatTemp *= 10;
		floatTemp += 0.5;
		v->value.decimal = (int)floatTemp;
	}
	
	
}

void handleCommand(char* commands)
{	
	int i,j,k,l;
	char cmd[10];
	char cond[30];
	char exp[30];
	if(commands[0] == '#')
	{
		for(i = 1; i < strlen(commands); i++)
		{		
			if(commands[i] == ' ' || commands[i] == '\n' || commands[i] == '[' || commands[i] == ':')
				continue;
			if(commands[i] == '*')
			{
				j= 0;
				i++;
				while(commands[i] != ':' && commands[i] != ']')
				{
					cmd[j] = commands[i];
					j++;
					i++;
				}
				cmd[j] = '\0';
			}
			else if(commands[i] == '@')
			{
				l=0;
				i++;
				while(commands[i] != ':' && commands[i] != ']')
				{
					cond[l] = commands[i];
					l++;
					i++;
				}
				cond[l] = '\0';
			}
			else if(commands[i] == '$')
			{
				k=0;
				i++;
				while(commands[i] != ':' && commands[i] != ']')
				{	
					exp[k] = commands[i];
					i++;
					k++;
				}
				exp[k] = '\0';			
			}
			if(commands[i] == '#' || commands[i] == ']')
				break;
		}
	}
	if(strcmp(cmd, "Make") == 0)
	{
		createVar(exp);
	}
	else if(strcmp(cmd, ";") == 0)
	{
		handleExpression(exp);
	}
	else if(strcmp(cmd, "Quit") == 0)
	{
		exit(EXIT_SUCCESS);
	}
	else if(strcmp(cmd,"Value") == 0)
	{
		Value(exp);
	}
	else if(strcmp(cmd,"AllValues") == 0)
	{
		printHash(symbolTable,tableSize);
	}
	else if(strcmp(cmd,"IF") == 0)
	{
		executeIf(cond,exp);
	}
	else if(strcmp(cmd,"Print") == 0)
	{
		print(exp);
	}
	else if(strcmp(cmd,"While") == 0)
	{
		loop(cond,exp);
	}
}

void Value(char* name)
{
	var* newv = searchHash(symbolTable, tableSize, name);
	if(newv != NULL)
		printf("%.2f\n",newv->value.value);
	else
		printf("variable not available\n");
}

int evaluateIf(char* exp)
{
	int i,j,k,z = 0,y;
	char name[30];
	char infixExp[100];
	//first
	for(i = 0; exp[i] != '<' ; i++)
	{
		if(exp[i] == ' ' || exp[i] == '\n')
			continue;
		if((exp[i]>= '0' && exp[i]<= '9')||exp[i] == '.')
		{
			infixExp[z] = exp[i];
			z++;
		}
		else if(isalpha(exp[i]))
		{
			j = 0;
			name[j] = exp[i];
			j++;
			i++;
			k = i+1;
			while(isalpha(exp[i]))
			{
				name[j] = exp[i];
				j++;
				i++;
			}
			i--;
			name[j] = '\0';
			var* temp = searchHash(symbolTable, tableSize, name);
			if(temp == NULL)
			{
				printf("variable %s not defined\n",name);
				return -1;
			}
			int numTemp = temp->value.integer;
			int numtemp = 0;
			
			while(numTemp > 0)
			{
				numtemp = numTemp  % 10;
				infixExp[z] = numtemp + '0';
				z++;
				numTemp /= 10;
			}
			infixExp[z] = '.';
			z++;
			numTemp = temp->value.decimal;
			numtemp = 0;

			for(y = 0; y < temp->value.decimalPlaces ; y++)
			{
				numtemp = numTemp  % 10;
				infixExp[z] = numtemp + '0';
				z++;
				numTemp /= 10;
			}
		}
		else
		{
			infixExp[z] = exp[i];
			z++;
		}
			
		
	}
	infixExp[z] = '\0';
	i++;

	double first;
	int error = 0;
	char postfixExp[100];
	convertInfix(infixExp, postfixExp, &error);
	if(error == 0)
	{
		first = evaluatePostFix(postfixExp, &error);
	}
	//comparator
	char comparator[8];
	int s = 0;
	for(i; exp[i] != '>' ; i++)
	{
		comparator[s] = exp[i];
		s++;
	}
	comparator[s] = '\0';
	i++;
	//second
	char name2[30];
	char infixExp2[100];
	z = 0;
	for(i ; i < strlen(exp) ; i++)
	{
		if(exp[i] == ' ' || exp[i] == '\n')
			continue;
		if((exp[i]>= '0' && exp[i]<= '9')||exp[i] == '.')
		{
			infixExp2[z] = exp[i];
			z++;
		}
		else if(isalpha(exp[i]))
		{
			j = 0;
			name2[j] = exp[i];
			j++;
			i++;
			k = i+1;
			while(isalpha(exp[i]))
			{
				name2[j] = exp[i];
				j++;
				i++;
			}
			i--;
			name2[j] = '\0';
			var* temp = searchHash(symbolTable, tableSize, name2);
			if(temp == NULL)
			{
				printf("variable %s not defined\n",name2);
				return -1;
			}
			int numTemp = temp->value.integer;
			int numtemp = 0;
			
			while(numTemp > 0)
			{
				numtemp = numTemp  % 10;
				infixExp2[z] = numtemp + '0';
				z++;
				numTemp /= 10;
			}
			infixExp2[z] = '.';
			z++;
			numTemp = temp->value.decimal;
			numtemp = 0;

			for(y = 0; y < temp->value.decimalPlaces ; y++)
			{
				numtemp = numTemp  % 10;
				infixExp2[z] = numtemp + '0';
				z++;
				numTemp /= 10;
			}
		}
		else
		{
			infixExp2[z] = exp[i];
			z++;
		}
			
		
	}
	infixExp2[z] = '\0';
	i++;

	double second;
	error = 0;
	char postfixExp2[100];
	convertInfix(infixExp2, postfixExp2, &error);
	if(error == 0)
	{
		second = evaluatePostFix(postfixExp2, &error);
	}

	if(strcmp(comparator,"biggerthan") == 0)
	{
		if(first > second)
			return 1;
		return 0;
	}

	if(strcmp(comparator,"smallerthan") == 0)
	{
		if(first < second)
			return 1;
		return 0;
	}

	if(strcmp(comparator,"equals") == 0)
	{
		if(first == second)
			return 1;
		return 0;
	}
}

void executeIf(char* condition,char* expressions)
{
	int flag = evaluateIf(condition);
	char newline[100];
	int i,j = 1;
	newline[0] = '{';
	if(flag)
	{
		for(i = 0; i < strlen(expressions);i++)
		{
			newline[j] = expressions[i];
			j++;
		}
		newline[j] = '}';
		char commands[100];
		separateCommand(newline, commands);
		handleCommand(commands);
	}
}

void loop(char* condition, char* expressions)
{
	START: ;
	int flag = evaluateIf(condition);
	char newline[100];
	int i,j = 1;
	newline[0] = '{';
	if(flag)
	{
		for(i = 0; i < strlen(expressions);i++)
		{
			newline[j] = expressions[i];
			j++;
		}
		newline[j] = '}';
		char commands[100];
		separateCommand(newline, commands);
		handleCommand(commands);
	}
	else 
		return;
	goto START;

}

void print(char* string)
{
	printf("%s\n",string);
}
