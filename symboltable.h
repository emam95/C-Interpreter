typedef struct
{
	int isDecimal;
	int integer;
	int decimal;
	int decimalPlaces;
	double value;
}Decimal;

typedef struct var
{
	char Name[20]; 
	int key;
	Decimal value;
	struct var* next;
}var;

typedef struct
{
	var* head;
	var* tail;
}listOfVar;

extern int tableSize;
extern listOfVar symbolTable[];

extern void initDecimal(Decimal *d);
extern void initlistOfVar(listOfVar* l);
extern void initHash(listOfVar hashTable[], int tableSize);
extern var* newVar(char* Name, Decimal data,int tableSize);
extern unsigned long long getHash(char* str);
extern int getIndex(char* str, int tableSize);
extern void insertHashTable(char* str,listOfVar hashTable[],int tableSize,Decimal value);
extern var* searchHash(listOfVar hashTable[], int tableSize, char* str);
void printHash(listOfVar hashTable[],int tableSize);
extern void createVar(char* exp);
extern void handleExpression(char* exp);
extern void handleCommand(char* commands);
extern void Value(char* name);
extern int evaluateIf(char* exp);
extern void executeIf(char* condition,char* expressions);
extern void loop(char* condition, char* expressions);
extern void print(char* string);
