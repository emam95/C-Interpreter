#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "lib.h"
#include "symboltable.h"

//Stack:

void initStack(Stack* s)
{
	s->top = NULL;
}

Node* createNode(double value)
{
	Node* n = (Node*)malloc(sizeof(Node));
	n->data = value;
	n->next = NULL;
	return n;
}

void push(Stack* s, double value)
{
	Node* n = createNode(value);
	n->next = s->top;
	s->top = n;
}

double pop(Stack* s)
{
	Node* n = s->top;
	double data = n->data;
	s->top = n->next;
	free(n);
	return data;
}

int IsEmpty(Stack* s)
{
	if(s->top == NULL)
		return 1;
	return 0;
}

double stackTop(Stack* s)
{
	return s->top->data;
}

//Evaluating Postfix:

double operation(char c,double x,double y)
{
	if(c == '+')
		return (x+y);
	if(c == '-')
		return (x-y);
	if(c == '*')
		return (x*y);
	if(c == '/')
		return (x/y);
	//if(c == '%')
		//return (x%y);
	if(c == '^')
		return pow(x,y);
}

double evaluatePostFix(char* expression, int* error)
{
	Stack s;
	int i;
	double x,y,value;
	Decimal temp;
	initStack(&s);
	for(i = 0; i < strlen(expression); i++)
	{
		if(expression[i] == ' ' || expression[i] == ':' ||  expression[i] == '\n')
			continue;
		if((expression[i]>= '0' && expression[i]<= '9') || expression[i] == '.')
		{
			initDecimal(&temp);
			if(expression[i]>= '0' && expression[i]<= '9')
			{
				temp.integer = expression[i] - '0';
				int j = i+1;
				while(expression[j]>= '0' && expression[j]<= '9')
				{
					temp.integer *= 10;
					temp.integer += (expression[j] - '0');
					j++;
					i++;
				}
				i++;
			}
			if(expression[i] == '.')
			{
				int k = i+1;
				while(expression[k]>= '0' && expression[k] <= '9')
				{
					temp.decimal *= 10;
					temp.decimal += (expression[k] - '0');
					k++;
					i++;
					temp.decimalPlaces++;
				}
			}
			temp.value = temp.integer + (temp.decimal / pow(10,temp.decimalPlaces));
			push(&s, temp.value);
		}
		else
		{
			if(expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '%' || expression[i] == '^')
			{
					if(!IsEmpty(&s))
						y = pop(&s);
					if(!IsEmpty(&s))
						x = pop(&s);
					value = operation(expression[i], x, y);
					push(&s, value);
			}
			else
			{
				printf("invalid operator: %c\n",expression[i]);
				*error = 1;
				return 0;
			}
		}
	}
	if(!IsEmpty(&s))
		value = pop(&s);
	return value;
}


//Converting Infix:

int priority(char c)
{
	if(c == '+' || c == '-')
		return 0;
	if(c == '*' || c == '/' || c == '%')
		return 1;
	if(c == '^')
		return 2;
	if(c == '(')
		return 3;
	return -1;
}

void convertInfix(char* exp, char* postfixExp, int* error)
{
	int i, j=0;
	Stack s;
	initStack(&s);
	int decimalpoint;
	for(i = 0; i < strlen(exp); i++)
	{
		decimalpoint = 0;
		if(exp[i] == ' ' ||  exp[i] == '\n')
			continue;
		if((exp[i]>= '0' && exp[i]<= '9')||exp[i] == '.')
		{
			if(exp[i] == '.')
				decimalpoint = 1;
			postfixExp[j] = exp[i];
			j++;
			int k = i+1;
			while((exp[k]>= '0' && exp[k]<= '9')||exp[k] == '.')
			{
				if(exp[k] == '.')
				{
					if(decimalpoint)
					{
						printf("Error:there are two decimal points");
						*error = 1;
						return;
					}
					decimalpoint = 1;	
				}
				postfixExp[j] = exp[k];
				j++;
				k++;
				i++;
			}
			postfixExp[j] = ':';
			j++;
		}
		else if(exp[i] == ')')
		{
			while(!IsEmpty(&s) && stackTop(&s) != '(')
			{
				postfixExp[j] = pop(&s);
				j++;
			}
			pop(&s);
		}
		else if(IsEmpty(&s))
			push(&s, exp[i]);
		else
		{
			while(!IsEmpty(&s) && priority(stackTop(&s)) >= priority(exp[i]) && stackTop(&s) != '(')
			{
				postfixExp[j] = pop(&s);
				j++;
			}
			push(&s,exp[i]);
		}
	}
	while(!IsEmpty(&s))
	{
		postfixExp[j] = pop(&s);
		j++;
	}
	postfixExp[j] = '\0';
}

//Others:

int countDecimal(double decimal)
{
	int temp = 0;
    	int count=0;

    	while (decimal != temp)
    	{
        	decimal = decimal * 10;
        	temp = decimal;
        	count++;
   	}
	return count;
}

int countChar(char* line, char c)
{
	int i,count = 0;
	for(i = 0; i < strlen(line) ; i++)
	{
		if(line[i] == 'c')
			count++;
	}
	return count;
}

void getInput(char* input)
{
	char buffer[256];
	if(fgets(buffer, sizeof(buffer), stdin))
	{
    		strcpy(input, buffer);
	}
}

void separateCommand(char* line, char* commands)
{
	int i = 0, j = 2, k;
	commands[0] = '#';
	commands[1] = '[';
	int n_cmds = 0, n_cond = 0, n_exp = 0;
	for(i = 0; i < strlen(line) ; i++)
	{	
		if(line[i] == ' ' || line[i] == '\n')
			continue;
		if(line[i] == '#')
		{
			if(j != 2)
			{
				commands[j] = ':';
				j++;
			}
			n_cmds++;
			k = i+1;
			commands[j] = '*';
			j++;
			while(line[k] != '(' && line[k] != '{' && line[k] != ';')
			{
				if(line[k] == ' ' || line[k] == '\n')
				{
					k++;
					i++;
					continue;
				}
				commands[j] = line[k];
				k++;
				j++;
				i++;
			}
		}
		else if(line[i] == '(')
		{
			if(j != 2)
			{
				commands[j] = ':';
				j++;
			}
			n_cond++;
			k = i + 1;
			commands[j] = '@';
			j++;
			while(line[k] != ')')
			{
				if(line[k] == ' ' || line[k] == '\n')
				{
					k++;
					i++;
					continue;
				}
				commands[j] = line[k];
				k++;
				i++;
				j++;
			}
		}
		else if(line[i] == '{')
		{
			if(j != 2)
			{
				commands[j] = ':';
				j++;
			}
			n_exp++;
			commands[j] = '$';
			j++;
			k = i + 1;
			while(line[k] != '}' && line[k] != ';')
			{
				if(line[k] == ' ' || line[k] == '\n')
				{
					k++;
					i++;
					continue;
				}
				commands[j] = line[k];
				k++;
				i++;
				j++;
			}
		}
		else if(line[i] == ';')
			break;
	}

	if(n_cmds == 0)
	{
		if(j != 2)
		{
			commands[j] = ':';
			j++;
		}
		commands[j] = '*';
		j++;
		commands[j] = ';';
		j++;
	}
	if(n_cond == 0)
	{
		if(j != 2)
		{
			commands[j] = ':';
			j++;
		}
		commands[j] = '@';
		j++;
		commands[j] = ';';
		j++;
	}
	if(n_exp == 0)
	{
		if(j != 2)
		{
			commands[j] = ':';
			j++;
		}
		commands[j] = '$';
		j++;
		commands[j] = ';';
		j++;
	}
	commands[j] = ']';
	j++;
	commands[j] = '#';
	j++;
	commands[j] = '\0';
}
