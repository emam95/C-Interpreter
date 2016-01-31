typedef struct Node
{
	double data;
	struct Node* next;
}Node;

//Stack:

typedef struct
{
	Node* top;
}Stack;

extern void initStack(Stack* s);
extern Node* createNode(double value);
extern void push(Stack* s, double value);
extern double pop(Stack* s);
extern int IsEmpty(Stack* s);
extern double stackTop(Stack* s);

//Evaluating Postfix:

extern double operation(char c, double x, double y);
extern double evaluatePostFix(char* expression, int* error);

//Converting Infix:

extern int priority(char c);
extern void convertInfix(char* exp, char* postfixExp,int* error);

//Others:

extern int countDecimal(double decimal);
extern int countChar(char* line, char c);
extern void getInput(char* input);
extern void separateCommand(char* line, char* commands);

/*
//to be implemented
extern void help();
extern void repeat();
extern void openFile();
extern void readFile();
extern void writeFile();
extern void newFun();
extern int isValidCommand();
*/
