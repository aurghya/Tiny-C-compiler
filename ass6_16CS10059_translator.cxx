#include "ass6_16CS10059_translator.h"
#include "y.tab.h"
#include<iostream>
#include<cstdlib>
#include<cstdio>

type_n *glob_type;
int glob_width;
int next_instr;
int temp_count=0;
symtab *glob_st =new symtab();
symtab *curr_st = new symtab();
quad_arr glob_quad;
vector <string> vs;
vector <string> cs;
int size_int=4;
int size_double=8;
int size_pointer=8;
int size_char=1;
int size_bool=1;
vector<string> strings_label;
type_n::type_n(types t,int sz,type_n *n)
{
	basetp=t;
	size=sz;
	next=n;
}

int type_n::getSize()
{
	if(this==NULL)
		return 0;
	//return the size of the array by calling the recursive function 
	//here we are not checking for null as if it will reach the final type it will enter the below conditions
	// if((this->basetp) == tp_arr)
	// 	return ((this->size)*(this->next->getSize()));
	// if((this->basetp) == tp_void)
	// 	return 0;
	// if((this->basetp) == tp_int)
	// 	return size_int;
	// if((this->basetp) == tp_double)
	// 	return size_double;
	// if((this->basetp) == tp_bool)
	// 	return size_bool;
	// if((this->basetp) == tp_char)
	// 	return size_char;
	// if((this->basetp) == tp_ptr)
	// 	return size_pointer;
	switch(this->basetp){
		case tp_arr:return ((this->size)*(this->next->getSize()));
		case tp_void:return 0;
		case tp_int:return size_int;
		case tp_char:return size_char;
		case tp_bool:return size_bool;
		case tp_double:return size_double;
		case tp_ptr:return size_pointer;
		default:return 0;
	}
}

types type_n::getBasetp()
{
	if(this!=NULL)
		return this->basetp;
	else
		return tp_void;
}

void type_n::printSize()
{
	printf("%d\n",size);
}

void type_n::print()
{
	if(basetp==tp_void)cout<<"Void";
	else if(basetp==tp_bool)cout<<"Bool";
	else if(basetp==tp_int)cout<<"Int";
	else if(basetp==tp_char)cout<<"Char";
	else if(basetp==tp_double)cout<<"Double";
	else if(basetp==tp_ptr){
		printf("ptr(");
		if(this->next!=NULL)
			this->next->print();
		printf(")");
	}
	else if(basetp==tp_arr){
		printf("array(%d,",size);
		if(this->next!=NULL)
			this->next->print();
		printf(")");		
	}
	else if(basetp==tp_func)cout<<"Function()";
	else{
		printf("TYPE NOT FOUND\n");
		exit(-1);		
	}
}

array::array(string s,int sz,types t):
	base_arr(s),
	tp(t),
	bsize(sz),
	dimension_size(1){};

void array::addindex(int i)
{
	this->dimension_size=this->dimension_size+1;
	this->dims.push_back(i);
}

void funct::print()
{

	cout<<"Funct(";
	int i;
	for(i=0;i<typelist.size();i++)
	{
		if(i!=0)
			cout<<",";
		cout<<typelist[i];
	}
	cout<<")";
}

funct::funct(vector<types> tpls)
{
	typelist=tpls;
}
symdata::symdata(string n)
{
	name=n;
	size=0;
	tp_n=NULL;
	offset=-1;
	var_type="";
	isInitialized=false;
	isFunction=false;
	isArray=false;
	ispresent=true;
	arr=NULL;
	fun=NULL;
	nest_tab=NULL;
	isdone=false;
	isptrarr=false;
	isGlobal=false;
}

void symdata::createarray()
{
	arr= new array(this->name,this->size,tp_arr);
}


symtab::symtab()
{
	name="";
	offset=0;
	no_params=0;
}

symtab::~symtab()
{
	int i=0;
	while(i<symbol_tab.size())
	{
		type_n *p1=symbol_tab[i]->tp_n;
		type_n *p2;
		while(true)
		{
			if(p1 == NULL)
				break;
			p2=p1;
			p1=p1->next;
			delete p2;
		}
		i++;
	}
}
int symtab::findg(string n)
{
	for(int i=0;i<vs.size();++i)
		if(vs[i]==n)
			return 1;
	for(int i=0;i<cs.size();++i)
		if(cs[i]==n)
			return 2;
	return 0;
}
type_n *CopyType(type_n *t)
{
	if(t == NULL) return t;
	type_n *ret = new type_n(t->basetp);

	ret->size = t->size;
	ret->basetp = t->basetp;

	ret->next = CopyType(t->next);
	return ret;
}

symdata* symtab::lookup(string n)
{
	int i=0,flag=0;
	while(i<symbol_tab.size())
	{
		if(symbol_tab[i]->name == n)
		{
			flag=1;
		}
		if(flag==1){
			return symbol_tab[i];
		}
		i++;
	}
	symdata *temp_o=new symdata(n);
	temp_o->i_val.int_val=0;
	symbol_tab.push_back(temp_o);
	return symbol_tab[symbol_tab.size()-1];
}

symdata* symtab::lookup_2(string n)
{
	int i=0,flag=0;
	while(i<symbol_tab.size())
	{
		if(symbol_tab[i]->name == n)
		{
			flag=1;
		}
		if(flag==1)return symbol_tab[i];
		i++;
	}
	i=0;
	while(i<glob_st->symbol_tab.size())
	{
		if(glob_st->symbol_tab[i]->name == n)
			return glob_st->symbol_tab[i];
		i++;
	}
	symdata *temp_o=new symdata(n);
	temp_o->i_val.int_val=0;
	symbol_tab.push_back(temp_o);
	return symbol_tab[symbol_tab.size()-1];
}

symdata* symtab::search(string n)
{
	int i=0,flag=0;
	while(i<symbol_tab.size())
	{
		if(symbol_tab[i]->name==n && symbol_tab[i]->ispresent)
		{
			flag=1;
		}
		if(flag==1){
			return symbol_tab[i];
		}
		i++;
	}
	
	return NULL;
}

symdata* symtab::gentemp(type_n *type)
{
	char c[10];
	sprintf(c,"t%03d",temp_count);
	temp_count++;
	symdata *temp=lookup(c);
	int temp_sz;
	if(type==NULL)
		temp_sz=0;
	switch(type->basetp){
		case tp_void:temp_sz=0;break;
		case tp_int:temp_sz=size_int;break;
		case tp_double:temp_sz=size_double;break;
		case tp_bool:temp_sz=size_bool;break;
		case tp_char:temp_sz=size_char;break;
		case tp_ptr:temp_sz=size_pointer;break;
		default:temp_sz=type->getSize();
	}
	temp->size=temp_sz;
	temp->var_type="temp";
	temp->tp_n=type;
	temp->offset=this->offset;
	this->offset=this->offset+(temp->size);
	return temp;
}

void symtab::update(symdata *sm,type_n *type,basic_val initval,symtab *next)
{
	sm->tp_n=CopyType(type);
	sm->i_val=initval;
	sm->nest_tab=next;
	int temp_sz;
	if(sm->tp_n==NULL)
		temp_sz=0;
	switch(type->basetp){
		case tp_void:temp_sz=0;break;
		case tp_int:temp_sz=size_int;break;
		case tp_double:temp_sz=size_double;break;
		case tp_bool:temp_sz=size_bool;break;
		case tp_char:temp_sz=size_char;break;
		case tp_ptr:temp_sz=size_pointer;break;
		default:temp_sz=sm->tp_n->getSize();
	}	
	sm->size=temp_sz;
	sm->offset=this->offset;
	this->offset=this->offset+(sm->size);
	sm->isInitialized=false;
}

void symtab::print()
{
	cout<<"=================================================================================================="<<endl;
	printf("Symbol Table : %s\n",name.c_str());
	printf("Offset = %d\nStart Quad Index = %d\nEnd Quad Index =  %d\n",offset,start_quad,end_quad);
	cout<<"------------------------------------------------------------------------------------------------"<<endl;
	printf("Name\tValue\tvar_type\tsize\tOffset\tType\n");
	cout<<"------------------------------------------------------------------------------------------------"<<endl;
	int i=0;
//	while(i<(symbol_tab).size())
    for(; i<(symbol_tab).size(); ++i)
    {
        if(!symbol_tab[i]->ispresent)continue;
        symdata * t = symbol_tab[i];
        printf("%s\t",symbol_tab[i]->name.c_str()); 
        if(t->isInitialized)
        {
        	if((t->tp_n)->basetp == tp_char) printf("%c\t",(t->i_val).char_val);
        	else if((t->tp_n)->basetp == tp_int) printf("%d\t",(t->i_val).int_val);
        	else if((t->tp_n)->basetp == tp_double) printf("%.3lf\t",(t->i_val).double_val);
       	 	else printf("----\t");
      	}
      	else
      		printf("null\t");
        printf("%s",t->var_type.c_str());
        printf("\t\t%d\t%d\t",t->size,-t->offset);
		if(t->var_type == "func")
			printf("ptr-to-St( %s )",t->nest_tab->name.c_str());

		if(t->tp_n != NULL)
			(t->tp_n)->print();
		printf("\n");
	}
	printf("\n");
	cout<<"=================================================================================================="<<endl;
}
list* makelist(int i)
{
	list *temp = (list*)malloc(sizeof(list));
	temp->index=i;
	temp->next=NULL;
	return temp;
}
list* merge(list *lt1,list *lt2)
{
	list *temp = (list*)malloc(sizeof(list));
	list *p1=temp;
	int flag=0;
	list *l1=lt1;
	list *l2=lt2;
//	while(l1!=NULL)
	for(;l1!=NULL;)
	{
		flag=1;
		p1->index=l1->index;
		if(l1->next!=NULL)
		{
			p1->next=(list*)malloc(sizeof(list));
			p1=p1->next;
		}
		l1=l1->next;
	}
//	while(l2!=NULL)
	for(;l2!=NULL;)
	{
		if(flag==1)
		{
			p1->next=(list*)malloc(sizeof(list));
			p1=p1->next;
			flag=0;
		}
		p1->index=l2->index;
		if(l2->next!=NULL)
		{
			p1->next=(list*)malloc(sizeof(list));
			p1=p1->next;
		}
		l2=l2->next;
	}
	p1->next=NULL;
	return temp;
}

quad::quad(opcode opc,string a1,string a2,string rs):
	op(opc),
	arg1(a1),
	result(rs),
	arg2(a2){};

void quad::print_arg()
{
	printf("\t%s\t=\t%s\top\t%s\t",result.c_str(),arg1.c_str(),arg2.c_str());
}

quad_arr::quad_arr()
{
	next_instr=0;
}

void quad_arr::emit(opcode opc, string arg1, string arg2, string result)
{
	if(result.size()!=0)
	{
		quad new_elem(opc,arg1,arg2,result);
		arr.push_back(new_elem);
	}
	else if(arg2.size()!=0)
	{
		quad new_elem(opc,arg1,"",arg2);
		arr.push_back(new_elem);
	}
	else if(arg1.size()!=0)
	{
		quad new_elem(opc,"","",arg1);
		arr.push_back(new_elem);
	}
	else
	{
		quad new_elem(opc,"","","");
		arr.push_back(new_elem);
	}
	next_instr=next_instr+1;
}
void quad_arr::emit2(opcode opc,string arg1, string arg2, string result)
{
	if(result.size()==0)
	{
		quad new_elem(opc,arg1,arg2,"");
		arr.push_back(new_elem);
	}
}
void quad_arr::emit(opcode opc, int val, string operand)
{
	char str[20];
	sprintf(str, "%d", val);
	if(operand.size()==0)
	{
		quad new_quad(opc,"","",str);
		arr.push_back(new_quad);
	}
	else
	{
		quad new_quad(opc,str,"",operand);
		arr.push_back(new_quad);
	}
	next_instr=next_instr+1;
}

void quad_arr::emit(opcode opc, double val, string operand)
{
	char str[20];
	sprintf(str, "%lf", val);
	if(operand.size()==0)
	{
		quad new_quad(opc,"","",str);
		arr.push_back(new_quad);
	}
	else
	{
		quad new_quad(opc,str,"",operand);
		arr.push_back(new_quad);
	}
	next_instr=next_instr+1;
}

void quad_arr::emit(opcode opc, char val, string operand)
{
	char str[20];
	sprintf(str, "'%c'", val);
	if(operand.size()==0)
	{
		quad new_quad(opc,"","",str);
		arr.push_back(new_quad);
	}
	else
	{
		quad new_quad(opc,str,"",operand);
		arr.push_back(new_quad);
	}
	next_instr=next_instr+1;
}

void quad_arr::print()
{
	opcode op;
	string arg1;
	string arg2;
	string result;
	int i=0;
//	for(int i=0;i<next_instr;i++)
	while(i<next_instr)
	{

		op=arr[i].op;
		arg1=arr[i].arg1;
		arg2=arr[i].arg2;
		result=arr[i].result;
		printf("%3d. :",i);
		if(Q_PLUS<=op && op<=Q_NOT_EQUAL)
	    {
	        printf("%s",result.c_str());
	        printf("\t=\t");
	        printf("%s",arg1.c_str());
	        printf(" ");
			if(op==Q_PLUS)cout<<"+";
			else if(op==Q_MINUS)cout<<"-";
			else if(op==Q_MULT)cout<<"*";
			else if(op==Q_DIVIDE)cout<<"/";
			else if(op==Q_MODULO)cout<<"%%";
			else if(op==Q_LEFT_OP)cout<<"<<";
			else if(op==Q_RIGHT_OP)cout<<">>";
			else if(op==Q_XOR)cout<<"^";
			else if(op==Q_AND)cout<<"&";
			else if(op==Q_OR)cout<<"|";
			else if(op==Q_LOG_AND)cout<<"&&";
			else if(op==Q_LOG_OR)cout<<"||";
			else if(op==Q_LESS)cout<<"<";
			else if(op==Q_LESS_OR_EQUAL)cout<<"<=";
			else if(op==Q_GREATER_OR_EQUAL)cout<<">=";
			else if(op==Q_GREATER)cout<<">";
			else if(op==Q_EQUAL)cout<<"==";
			else if(Q_NOT_EQUAL)cout<<"!=";
	        printf(" ");
	       	printf("%s\n",arg2.c_str());
	    }
	    else if(Q_UNARY_MINUS<=op && op<=Q_ASSIGN)
	    {
	        printf("%s",result.c_str());
	        printf("\t=\t");
			if(op==Q_UNARY_MINUS)cout<<"-";
			else if(op==Q_UNARY_PLUS)cout<<"+";
			else if(op==Q_COMPLEMENT)cout<<"~";
			else if(op==Q_NOT)cout<<"!";
	        printf("%s\n",arg1.c_str());
	    }
	    else if(op == Q_GOTO){printf("goto ");printf("%s\n",result.c_str());}
	    else if(Q_IF_EQUAL<=op && op<=Q_IF_GREATER_OR_EQUAL)
	    {
	        printf("if  ");printf("%s",arg1.c_str());printf(" ");
			if(op==Q_IF_LESS)cout<<"<";
			else if(op==Q_IF_GREATER)cout<<">";
			else if(op==Q_IF_LESS_OR_EQUAL)cout<<"<=";
			else if(op==Q_IF_GREATER_OR_EQUAL)cout<<">=";
			else if(op==Q_IF_EQUAL)cout<<"==";
			else if(op==Q_IF_NOT_EQUAL)cout<<"!=";
			else if(op==Q_IF_EXPRESSION)cout<<"!= 0";
			else if(op==Q_IF_NOT_EXPRESSION)cout<<"== 0";
	        printf("%s",arg2.c_str());printf("\tgoto ");printf("%s\n",result.c_str());            
	    }
	    else if(Q_CHAR2INT<=op && op<=Q_DOUBLE2INT)
	    {
	        printf("%s",result.c_str());printf("\t=\t");      
			if(op==Q_CHAR2INT)  {printf(" Char2Int(");printf("%s",arg1.c_str());printf(")\n");}
			else if(op==Q_CHAR2DOUBLE) {printf(" Char2Double(");printf("%s",arg1.c_str());printf(")\n");}
			else if(op==Q_INT2CHAR) {printf(" Int2Char(");printf("%s",arg1.c_str());printf(")\n");}
			else if(op==Q_DOUBLE2CHAR) {printf(" Double2Char(");printf("%s",arg1.c_str());printf(")\n");}
			else if(op==Q_INT2DOUBLE) {printf(" Int2Double(");printf("%s",arg1.c_str());printf(")\n");}
			else if(op==Q_DOUBLE2INT) {printf(" Double2Int(");printf("%s",arg1.c_str());printf(")\n");}
	    }
	    else if(op == Q_PARAM)
	    {
	        printf("param\t");printf("%s\n",result.c_str());
	    }
	    else if(op == Q_CALL)
	    {
	        if(!result.c_str())
					printf("call %s, %s\n", arg1.c_str(), arg2.c_str());
			else if(result.size()==0)
			{
				printf("call %s, %s\n", arg1.c_str(), arg2.c_str());
			}
			else
				printf("%s\t=\tcall %s, %s\n", result.c_str(), arg1.c_str(), arg2.c_str());
	    }
	    else if(op == Q_RETURN)
	    {
	        printf("return\t");printf("%s\n",result.c_str());
	    }
	    else if( op == Q_RINDEX)
	    {
	        printf("%s\t=\t%s[%s]\n", result.c_str(), arg1.c_str(), arg2.c_str());
	    }
	    else if(op == Q_LINDEX)
	    {
	        printf("%s[%s]\t=\t%s\n", result.c_str(), arg1.c_str(), arg2.c_str());
	    }
	    else if(op == Q_LDEREF)
	    {
	        printf("*%s\t=\t%s\n", result.c_str(), arg1.c_str());
	    }
	    else if(op == Q_RDEREF)
	    {
	    	printf("%s\t=\t* %s\n", result.c_str(), arg1.c_str());
	    }
	    else if(op == Q_ADDR)
	    {
	    	printf("%s\t=\t& %s\n", result.c_str(), arg1.c_str());
	    }
		i++;
	}
}

void backpatch(list *l,int i)
{
	list *temp =l;
	list *temp2;
	char str[10];
	sprintf(str,"%d",i);
	while(temp!=NULL)
	{
		glob_quad.arr[temp->index].result = str;
		temp2=temp;
		temp=temp->next;
		free(temp2);
	}
}

void typecheck(expresn *e1,expresn *e2,bool isAssign)
{
	types type1,type2;
	//if(e2->type)
	if(e1->type==NULL)
	{
		e1->type = CopyType(e2->type);
	}
	else if(e2->type==NULL)
	{
		e2->type =CopyType(e1->type);
	}
	type1=(e1->type)->basetp;
	type2=(e2->type)->basetp;
	if(type1==type2)
	{
		return;
	}
	if(!isAssign)
	{
		if(type1>type2)
		{
			symdata *temp = curr_st->gentemp(e1->type);
			if(type1 == tp_int && type2 == tp_char)
				glob_quad.emit(Q_CHAR2INT, e2->loc->name, temp->name);
			else if(type1 == tp_double && type2 == tp_int)
				glob_quad.emit(Q_INT2DOUBLE, e2->loc->name, temp->name);
			e2->loc = temp;
			e2->type = temp->tp_n;
		}
		else
		{
			symdata *temp = curr_st->gentemp(e2->type);
			if(type2 == tp_int && type1 == tp_char)
				glob_quad.emit(Q_CHAR2INT, e1->loc->name, temp->name);
			else if(type2 == tp_double && type1 == tp_int)
				glob_quad.emit(Q_INT2DOUBLE, e1->loc->name, temp->name);	
			e1->loc = temp;
			e1->type = temp->tp_n;
		}		
	}
	else
	{
		symdata *temp = curr_st->gentemp(e1->type);
		if(type1 == tp_int && type2 == tp_double)
			glob_quad.emit(Q_DOUBLE2INT, e2->loc->name, temp->name);
		else if(type1 == tp_double && type2 == tp_int)
			glob_quad.emit(Q_INT2DOUBLE, e2->loc->name, temp->name);
		else if(type1 == tp_char && type2 == tp_int)
			glob_quad.emit(Q_INT2CHAR, e2->loc->name, temp->name);
		else if(type1 == tp_int && type2 == tp_char)
			glob_quad.emit(Q_CHAR2INT, e2->loc->name, temp->name);
		else
		{
			printf("%s %s Types compatibility not defined\n",e1->loc->name.c_str(),e2->loc->name.c_str());
			exit(-1);
		}
		e2->loc = temp;
		e2->type = temp->tp_n;
	}
}

void print_list(list *root)
{
	int flag=0;
	while(root!=NULL)
	{
		printf("%d ",root->index);
		flag=1;
		root=root->next;
	}
	if(flag==0)
	{
		printf("Empty List\n");
	}
	else
	{
		printf("\n");
	}
}
void conv2Bool(expresn *e)
{
	if((e->type)->basetp!=tp_bool)
	{
		(e->type) = new type_n(tp_bool);
		e->falselist=makelist(next_instr);
		glob_quad.emit(Q_IF_EQUAL,e->loc->name,"0","-1");
		e->truelist = makelist(next_instr);
		glob_quad.emit(Q_GOTO,-1);
	}
}

void func_printi(){
	symdata *temp_printi=new symdata("printInt");
	temp_printi->tp_n=new type_n(tp_int);
	temp_printi->var_type="func";
	temp_printi->nest_tab=glob_st;
	glob_st->symbol_tab.push_back(temp_printi);
}

void func_readi(){
	symdata *temp_readi=new symdata("readInt");
	temp_readi->tp_n=new type_n(tp_int);
	temp_readi->var_type="func";
	temp_readi->nest_tab=glob_st;
	glob_st->symbol_tab.push_back(temp_readi);
}

void func_prints(){
	symdata *temp_prints=new symdata("printStr");
	temp_prints->tp_n=new type_n(tp_int);
	temp_prints->var_type="func";
	temp_prints->nest_tab=glob_st;
	glob_st->symbol_tab.push_back(temp_prints);
}

int main()
{
	func_printi();
	func_readi();
	func_prints();

	yyparse();
	glob_st->name="Global";
	printf("==============================================================================");
	printf("\nGenerated Quads for the program\n");
	glob_quad.print();
	printf("==============================================================================\n");
	printf("Symbol table Maintained For the Given Program\n");
	glob_st->print();
	set<string> setty;
	setty.insert("Global");

	printf("=============================================================================\n");
	FILE *fp;
	fp = fopen("output.s","w");
	fprintf(fp,"\t.file\t\"output.s\"\n");
	//Print the data of the strings here
	int i=0;
	while( i < strings_label.size())
	{
		fprintf(fp,"\n.STR%d:\t.string %s",i,strings_label[i].c_str());	
		i++;
	}
	set<string>setty_1;
	glob_st->mark_labels();
	glob_st->global_variables(fp);
	setty_1.insert("Global");
	int count_l=0;
	i=0;
	while(i < glob_st->symbol_tab.size())
	{
		if(((glob_st->symbol_tab[i])->nest_tab)!=NULL)
		{
			if(setty_1.find(((glob_st->symbol_tab[i])->nest_tab)->name)==setty_1.end())
			{
				glob_st->symbol_tab[i]->nest_tab->assign_offset();
				glob_st->symbol_tab[i]->nest_tab->print();
				glob_st->symbol_tab[i]->nest_tab->function_prologue(fp,count_l);
				glob_st->symbol_tab[i]->nest_tab->function_restore(fp);
				glob_st->symbol_tab[i]->nest_tab->gen_internal_code(fp,count_l);
				setty_1.insert(((glob_st->symbol_tab[i])->nest_tab)->name);
				glob_st->symbol_tab[i]->nest_tab->function_epilogue(fp,count_l,count_l);
				count_l++;
			}
		}
		i++;
	}
	fprintf(fp,"\n");
	return 0;
}
