#ifndef ASS6_16CS10059_TRANSLATOR_H
#define ASS6_16CS10059_TRANSLATOR_H

#define INT_SIZE 4;
#define DOUBLE_SIZE 8;
#define POINTER_SIZE 8;
#define CHAR_SIZE 1;
#define BOOL_SIZE 1;

#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <string>
#include <set>
#include <map>
#include <stack>
using namespace std;
extern map<int,int> mp_set;
extern int size_int;
extern int size_double;
extern int size_pointer;
extern int size_char;
extern int size_bool;
extern stack<string> params_stack;
extern stack<int> types_stack;
extern stack<int> offset_stack;
extern stack<int> ptrarr_stack;
extern vector<string> strings_label;

class type_n;// to store a type
class expresn;// to be used for truelist,falselist and nextlist
class quad; //to define quad
class symdata; //data into symbol table
class symtab; //symbol table
class quad_arr; //quad array
class funct;
class array;
struct decStr;
struct idStr;
struct expresn;
struct arglistStr;
extern type_n *glob_type; 
extern int glob_width; 
extern int next_instr; 
extern int temp_count; 
extern symtab *glob_st;
extern symtab *curr_st; 
extern quad_arr glob_quad;

enum types{
	tp_void=0,tp_bool,tp_char,tp_int,tp_double,tp_ptr,tp_arr,tp_func
};

typedef struct list{
	int index;
	struct list *next;
}list;

enum opcode{

	//Binary Assignment Operator
	Q_PLUS=1,Q_MINUS,Q_MULT,Q_DIVIDE,Q_MODULO,Q_LEFT_OP,Q_RIGHT_OP,
	Q_XOR,Q_AND,Q_OR,Q_LOG_AND,Q_LOG_OR,Q_LESS,Q_LESS_OR_EQUAL,
	Q_GREATER_OR_EQUAL,Q_GREATER,Q_EQUAL,Q_NOT_EQUAL,

	//Unary Assignment Operator
	Q_UNARY_MINUS,Q_UNARY_PLUS,Q_COMPLEMENT,Q_NOT,

	//Copy Assignment
	Q_ASSIGN,

	//Unconditional Jump
	Q_GOTO,

	//Conditional Jump
	Q_IF_EQUAL,Q_IF_NOT_EQUAL,Q_IF_EXPRESSION,Q_IF_NOT_EXPRESSION,
	Q_IF_LESS,Q_IF_GREATER,Q_IF_LESS_OR_EQUAL,Q_IF_GREATER_OR_EQUAL,

	//Type Conversions
	Q_CHAR2INT,Q_CHAR2DOUBLE,Q_INT2CHAR,Q_DOUBLE2CHAR,Q_INT2DOUBLE,Q_DOUBLE2INT,


	//Procedure Call
	Q_PARAM,Q_CALL,Q_RETURN,

	//Pointer Assignment Operator
	Q_LDEREF,Q_RDEREF,
	Q_ADDR,

	//Array Indexing
	Q_RINDEX,
	Q_LINDEX,

};

//it is the basic type that an element can have
union basic_val{
	int int_val;
	double double_val;
	char char_val;
};

class type_n{
public:
	int size;    // to save the size of the type
	types basetp; // to save the basic type of the elemnt
	type_n *next; // to save next type_n type for arrays
	type_n(types t,int sz=1, type_n *n=NULL); //constuctor
	int getSize(); //returns the size
	types getBasetp(); //return Base type
	void printSize(); //to print the size
	void print(); 
};

type_n *CopyType(type_n *t);

class array
{
public:

	/* Stores the array base and the variable containing array offset */
	string base_arr;
	types tp;
	/* Initiates array name, offset and array type */
	array(string s,int sz,types t);
	/* Stores array dimensions */
	vector<int> dims;

	/* Size of base type */
	int bsize;

	/* Number of dimensions */
	int dimension_size;

	/* Adds an array index for array accessing */
	void addindex(int i);

};



/* To store details of functions, its parameters and return type */
class funct
{
public:
	/* Parameter type list */
	vector<types> typelist;

	/* Return type */
	type_n *rettype;

	funct(vector<types> tpls);
	/* Prints details in suitable format */
	void print();
};


//class which will be used as building element for symbol table
class symdata{
public:
	string name;//to store the value of a varaible stored in symbol table
	int size; //to stoe the size of an element in a symbol table
	int offset;//to maintain the offset at this variable to know the position at symbol table
	basic_val i_val;//to store the initialized value for an element stored at symbol table
	type_n *tp_n;//for storing the type of element
	symtab *nest_tab; //to store the pointer to the symbol table to which the current element belongs to
	array *arr;//to store the pointer to an array if its an array type
	funct *fun;//to store the pointer to a function if its an function
	void createarray();
	string var_type;//to store whether the varaible is "null=0" "local=1" "param=2" "func=3" "ret=4" "temporary=5"
	bool isInitialized; //If the value of element is initialized or not
	bool isFunction; //to know whether the current element is function like function pointer
	bool isArray; // to know whether the current element is ab array it helps if we have been in grammar tree
	symdata(string n=""); //name is initialized to null that will be used for naming temporary variables
	bool ispresent;
	bool isdone;
	bool isptrarr;
	bool isGlobal;
};	

class symtab{
public:
	string name;			// name of the symbol
	int offset;				// final offset of this symbol table that will be used in the update function
	int start_quad;
	int end_quad;
	vector<symdata*> symbol_tab; //maintaining a list of symbol tables
	symtab();  //constructor
	~symtab(); //destructor
	symdata* lookup(string n);// Lookup function searches the variable with name. If the variable is present then returns its pointer location else creates a new entry with its name and returns that pointer
	symdata* lookup_2(string n);//To handle global variables
	symdata* search(string n); //it searches for the variable and returns the oiter to it if present
	symdata* gentemp(type_n *type); //gentemp creates a new element in the symbol table with the type provided at the time of constructing
	void update(symdata *loc,type_n *type,basic_val initval, symtab *next = NULL);//
	void print();
	int no_params;
	void mark_labels();
	void function_prologue(FILE *fp,int count);
	void global_variables(FILE *fp);
	void gen_internal_code(FILE *fp,int ret_count);
	int function_call(FILE *fp);
	void function_epilogue(FILE *fp,int count,int ret_count);
	string assign_reg(int type_of,int no);
	void assign_offset();
	void function_restore(FILE *fp);
	int findg(string n);
};

struct expresn{
	symdata* loc;
	type_n* type;
	list* truelist;
	list* falselist;
	bool isPointer;
	bool isArray;
	bool isString;
	int ind_str;
	symdata *arr;
	symdata *poss_array;
};
list* makelist(int i);  //creates a list have only one element as i
list* merge(list *l1,list *l2); //merges the the two list and return a merged list
void backpatch(list *l,int i);  //to fill the dangling list of goto's l1 to i
void conv2Bool(expresn *e); //to convert the given exprssion type to bool mostly used in relational operator
void typecheck(expresn *e1,expresn *e2,bool isAss = false);
void print_list(list *root);
// struct for declaration grammar
struct decStr
{
	type_n *type;						// type of the current declaration
	int width;					// width of the variable
};

struct arglistStr
{
	vector<expresn*> *arguments;		// A simple vector is used to store the locations of all seen arguments
};

// struct for a identifier
struct idStr
{
	symdata *loc;					// pointer to the symboltable
	string *name;						// name of the identifier
};

struct strstr{
	type_n lop;
	string name;
};

class quad{
public:
	string arg1,result,arg2; //consist of three elements 
	opcode op;
	void print_arg();
	quad(opcode,string,string,string); //constructorparameters
};


class quad_arr{
public:
	vector<quad> arr;//to store the list of quads
	quad_arr();
	void emit(opcode opc, string arg1="", string arg2="", string result=""); //emit used for operations with 3 inputs
	void emit(opcode opc, int val, string operand="");  // emit for assignment of integer
	void emit(opcode opc, double val, string operand=""); //emit for assignment of double
	void emit(opcode opc, char val, string operand=""); //emit for assignment of char
	void emit2(opcode opc,string arg1="", string arg2="", string result="");
	void print();  //print all the quads
};

#endif
