#include "ass6_16CS10059_translator.h"
#include "y.tab.h"

extern quad_arr glob_quad;
extern int next_instr;
map<int,int> mp_set;
stack<string> params_stack;
stack<int> types_stack;
stack<int> offset_stack;
stack<int> ptrarr_stack;
extern std::vector< string > vs;
extern std::vector<string> cs;
int add_off;
void symtab::mark_labels()
{
	int count=1;
	for(int i=0;i<next_instr;i++)
	{
		switch(glob_quad.arr[i].op)
		{
			case Q_GOTO:
			case Q_IF_EQUAL:
			case Q_IF_NOT_EQUAL:
			case Q_IF_EXPRESSION:
			case Q_IF_NOT_EXPRESSION:
			case Q_IF_LESS:
			case Q_IF_GREATER:
			case Q_IF_LESS_OR_EQUAL:
			case Q_IF_GREATER_OR_EQUAL:
			if(glob_quad.arr[i].result!="-1")
			{	
				if(mp_set.find(atoi(glob_quad.arr[i].result.c_str()))==mp_set.end())
				{
					mp_set[atoi(glob_quad.arr[i].result.c_str())]=count;
					count++;				
				}
			}
		}
	}
}

void symtab::function_prologue(FILE *fp,int count)
{
	fprintf(fp,"\n\t.globl\t%s",name.c_str());
	fprintf(fp,"\n\t.type\t%s, @function",name.c_str());
	fprintf(fp,"\n%s:",name.c_str());
	fprintf(fp,"\n.LFB%d:",count);
	fprintf(fp,"\n\tpushq\t%%rbp\n\tmovq\t%%rsp, %%rbp");
	int t=-offset;
	fprintf(fp,"\n\tsubq\t$%d, %%rsp",t);
}

void symtab::function_epilogue(FILE *fp,int count,int ret_count)
{
	fprintf(fp,"\n.LRT%d:",ret_count);
	fprintf(fp,"\n\taddq\t$%d, %%rsp",offset);
	fprintf(fp,"\n\tmovq\t%%rbp, %%rsp\n\tpopq\t%%rbp\n\tret");
	fprintf(fp,"\n.LFE%d:",count);
	fprintf(fp,"\n\t.size\t%s, .-%s",name.c_str(),name.c_str());
}

void symtab::global_variables(FILE *fp)
{
	for(int i=0;i<symbol_tab.size();i++)
	{
		if(symbol_tab[i]->name[0]!='t' &&symbol_tab[i]->tp_n!=NULL&&symbol_tab[i]->var_type!="func")
		{
			if(symbol_tab[i]->tp_n->basetp==tp_int)
			{
				vs.push_back(symbol_tab[i]->name);
				switch(symbol_tab[i]->isInitialized){
					case false:{fprintf(fp,"\n\t.comm\t%s,4,4",symbol_tab[i]->name.c_str());break;}
					case true:{
						fprintf(fp,"\n\t.globl\t%s",symbol_tab[i]->name.c_str());
						fprintf(fp,"\n\t.data\n\t.align 4\n\t.type\t%s, @object",symbol_tab[i]->name.c_str());
						fprintf(fp,"\n\t.size\t%s ,4\n%s:\n\t.long %d",symbol_tab[i]->name.c_str(),symbol_tab[i]->name.c_str(),symbol_tab[i]->i_val.int_val);						
					}
				}
		    }
		    if(symbol_tab[i]->tp_n->basetp==tp_char)
			{
				cs.push_back(symbol_tab[i]->name);
				if(symbol_tab[i]->isInitialized==false)
				{
					fprintf(fp,"\n\t.comm\t%s,1,1",symbol_tab[i]->name.c_str());
				}
				else
				{
					fprintf(fp,"\n\t.globl\t%s",symbol_tab[i]->name.c_str());
					fprintf(fp,"\n\t.data\n\t.type\t%s, @object",symbol_tab[i]->name.c_str());
					fprintf(fp,"\n\t.size\t%s ,1\n%s:\n\t.byte %c",symbol_tab[i]->name.c_str(),symbol_tab[i]->name.c_str(),symbol_tab[i]->i_val.char_val);
				}
		    }
		}

	}
	fprintf(fp,"\n\t.text");
}
void symtab::assign_offset()
{
	int curr_offset=0,param_offset=16,flag=0;;
	no_params=0;
	for(int i = (symbol_tab).size()-1; i>=0; i--)
    {
        if(symbol_tab[i]->ispresent==false)
        	continue;
        if(symbol_tab[i]->var_type=="param" && symbol_tab[i]->isdone==false)
        {
        	no_params++;
			bool con=symbol_tab[i]->tp_n && symbol_tab[i]->tp_n->basetp==tp_arr;
        	if(con)
        	{
        		if(symbol_tab[i]->tp_n->size==-1)
        		{
					flag=1;
        		}
        		symbol_tab[i]->size=8;
				if(flag==1){
					symbol_tab[i]->isptrarr=true;
				}
				flag=0;
        	}
			int s=curr_offset-symbol_tab[i]->size;
        	symbol_tab[i]->offset=curr_offset=s;
        	symbol_tab[i]->isdone=true;
        }
        if(no_params==6)
        	break;
    }
    for(int i = 0; i<(symbol_tab).size(); i++)
    {
        if(!symbol_tab[i]->ispresent)continue;
		bool con1=symbol_tab[i]->var_type!="return"&&symbol_tab[i]->var_type!="param" && symbol_tab[i]->isdone==false;
		bool con2=symbol_tab[i]->var_type=="param" && symbol_tab[i]->isdone==false;
        if(con1)
        {
			int s=curr_offset-symbol_tab[i]->size;
        	symbol_tab[i]->offset=curr_offset=s;
        	symbol_tab[i]->isdone=true;
        }
        else if(con2)
        {
        	if(symbol_tab[i]->tp_n && symbol_tab[i]->tp_n->basetp==tp_arr)
        	{
        		if(symbol_tab[i]->tp_n->size==-1)
        		{
        			symbol_tab[i]->isptrarr=true;
					symbol_tab[i]->size=8;
        		}
        		else symbol_tab[i]->size=8;
        	}
        	symbol_tab[i]->isdone=true;
        	++no_params;
        	symbol_tab[i]->offset=param_offset;
        	param_offset=symbol_tab[i]->size+param_offset;
        }
    }
    offset=curr_offset;
}
string symtab::assign_reg(int type_of,int no)
{
	string s="NULL";
	if(type_of==tp_char){
		if(no==0)s="dil";
		else if(no==1)s="sil";
		else if(no==2)s="dl";
		else if(no==3)s="cl";
		else if(no==4)s="r8b";
		else if(no==5)s="r9b";
    }
    else if(type_of == tp_int){
		if(no==0)s="edi";
		else if(no==1)s="esi";
		else if(no==2)s="edx";
		else if(no==3)s="ecx";
		else if(no==4)s="r8d";
		else if(no==5)s="r9d";
    }
    else
    {
		if(no==0)s="rdi";
		else if(no==1)s="rsi";
		else if(no==2)s="rdx";
		else if(no==3)s="rcx";
		else if(no==4)s="r8";
		else if(no==5)s="r9";

    }
    return s;
}
int symtab::function_call(FILE *fp)
{
	
	int c=0;
	fprintf(fp,"\n\tpushq %%rbp");
	int count=0;
	while(count<6 && params_stack.size())
	{
		string p=params_stack.top();params_stack.pop();
		int btp=types_stack.top();types_stack.pop();
		int off=offset_stack.top();offset_stack.pop();
		int parr=ptrarr_stack.top();ptrarr_stack.pop();
		
		string temp_str=assign_reg(btp,count);
		if(temp_str!="NULL")
		{
			switch(btp){
				case tp_int:{fprintf(fp,"\n\tmovl\t%d(%%rbp) , %%%s",off,temp_str.c_str());break;}
				case tp_char:{fprintf(fp,"\n\tmovb\t%d(%%rbp), %%%s",off,temp_str.c_str());break;}
				case tp_arr:{
					if(parr==1)fprintf(fp,"\n\tmovq\t%d(%%rbp), %%%s",off,temp_str.c_str());
					else fprintf(fp,"\n\tleaq\t%d(%%rbp), %%%s",off,temp_str.c_str());
					break;
				}
				default:{fprintf(fp,"\n\tmovq\t%d(%%rbp), %%%s",off,temp_str.c_str());break;}
			}
			count++;
		}
	}
	while(params_stack.size())
	{

		string p=params_stack.top();params_stack.pop();
		int btp=types_stack.top();types_stack.pop();
		int off=offset_stack.top();offset_stack.pop();
		int parr=ptrarr_stack.top();ptrarr_stack.pop();
		
		switch(btp){
			case tp_int:{
				fprintf(fp,"\n\tsubq $4, %%rsp\n\tmovl\t%d(%%rbp), %%eax\n\tmovl\t%%eax, (%%rsp)",off);
				c+=4;break;			
			}
			case tp_arr:{
				if(parr==1){
					fprintf(fp,"\n\tsubq $8, %%rsp\n\tmovq\t%d(%%rbp), %%rax\n\tmovq\t%%rax, (%%rsp)",off);
					c+=8;break;			
				}
				else {
					fprintf(fp,"\n\tsubq $8, %%rsp\n\tleaq\t%d(%%rbp), %%rax\n\tmovq\t%%rax, (%%rsp)",off);
					c+=8;break;					
				}
			}
			case tp_char:{
				fprintf(fp,"\n\tsubq $4, %%rsp\n\tmovsbl\t%d(%%rbp), %%eax\n\tmovl\t%%eax, (%%rsp)",off);
				c+=4;break;				
			}
			default:{
				fprintf(fp,"\n\tsubq $8, %%rsp\n\tmovq\t%d(%%rbp), %%rax\n\tmovq\t%%rax, (%%rsp)",off);
				c+=8;break;
			}
		}
	}
	return c;
	
}
void symtab::function_restore(FILE *fp)
{
	int count=0;
	string regname;
	for(int i=symbol_tab.size()-1;i>=0;i--)
	{
	    if(!symbol_tab[i]->ispresent)continue;
	    if(symbol_tab[i]->var_type=="param" && symbol_tab[i]->offset<0)
	    {
			switch(symbol_tab[i]->tp_n->basetp){
				case tp_char:{
					regname = assign_reg(tp_char,count);
					fprintf(fp,"\n\tmovb\t%%%s, %d(%%rbp)",regname.c_str(),symbol_tab[i]->offset);
					break;
				}
				case tp_int:{
					regname = assign_reg(tp_int,count);
					fprintf(fp,"\n\tmovl\t%%%s, %d(%%rbp)",regname.c_str(),symbol_tab[i]->offset);
					break;
				}
				default:{
					regname = assign_reg(10,count);
					fprintf(fp,"\n\tmovq\t%%%s, %d(%%rbp)",regname.c_str(),symbol_tab[i]->offset);
					break;
				}
			}
	    	count++;
	    }
	    if(count>=6)break;
    }
}
void symtab::gen_internal_code(FILE *fp,int ret_count)
{
	int i;				
	for(i = start_quad; i <=end_quad; ++i)
	{
		opcode &opx =glob_quad.arr[i].op;
		string &arg1x =glob_quad.arr[i].arg1;
		string &arg2x =glob_quad.arr[i].arg2;
		string &resx =glob_quad.arr[i].result;
		int offr,off1,off2,flag1=1,flag2=1,flag3=1,j;
		fprintf(fp,"\n# %d:",i);
		if(search(resx))
		{
			offr = search(resx)->offset;
			fprintf(fp,"res = %s ",search(resx)->name.c_str());
		}
		else if(glob_quad.arr[i].result!=""&& findg(glob_quad.arr[i].result))
		{
			flag3=0;
		}
		if(search(arg1x))
		{
		
			off1 = search(arg1x)->offset;
			fprintf(fp,"arg1 = %s ",search(arg1x)->name.c_str());
		}
		else if(glob_quad.arr[i].arg1!="" && findg(glob_quad.arr[i].arg1))
		{
			
				flag1=0;
				
		}
		if(search(arg2x))
		{
			off2 = search(arg2x)->offset;
			fprintf(fp,"arg2 = %s ",search(arg2x)->name.c_str());
		}
		else if(glob_quad.arr[i].arg2!="" && findg(glob_quad.arr[i].arg2))
		{
			
				flag2=0;
				
				
		}
		if(flag1==0)
		{
			if(findg(arg1x)==2)
					fprintf(fp,"\n\tmovzbl\t%s(%%rip), %%eax",arg1x.c_str());
				else
					fprintf(fp,"\n\tmovl\t%s(%%rip), %%eax",arg1x.c_str());
		}
		if(flag2==0)
		{
			if(findg(arg1x)==2)
					fprintf(fp,"\n\tmovzbl\t%s(%%rip), %%edx",arg2x.c_str());
				else
					fprintf(fp,"\n\tmovl\t%s(%%rip), %%edx",arg2x.c_str());
		}
		if(mp_set.find(i)!=mp_set.end())
		{
			fprintf(fp,"\n.L%d:",mp_set[i]);
		}
			if(opx==Q_PLUS){
				if(search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_char)
				{
					if(flag1!=0)
						fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off1);
					if(flag2!=0)
						fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%edx",off2);
					fprintf(fp,"\n\taddl\t%%edx, %%eax");
					if(flag3!=0)
						fprintf(fp,"\n\tmovb\t%%al, %d(%%rbp)",offr);
					else
						fprintf(fp,"\n\tmovb\t%%al, %s(%%rip)",resx.c_str());
				}
				else 
				{
					if(flag1!=0)
						fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off1);
					if(flag2!=0)
					{if(arg2x[0]>='0' && arg2x[0]<='9')
						fprintf(fp,"\n\tmovl\t$%s, %%edx",arg2x.c_str());
					else
						fprintf(fp,"\n\tmovl\t%d(%%rbp), %%edx",off2);
					}
					fprintf(fp,"\n\taddl\t%%edx, %%eax");
					if(flag3!=0)
						fprintf(fp,"\n\tmovl\t%%eax, %d(%%rbp)",offr);
					else
						fprintf(fp,"\n\tmovl\t%%eax, %s(%%rip)",resx.c_str());
				}
			}
			else if(opx==Q_MINUS){
				if(search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_char)
				{
					if(flag1!=0)
						fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off1);
					if(flag2!=0)
						fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%edx",off2);
					fprintf(fp,"\n\tsubl\t%%edx, %%eax");
					if(flag3!=0)
						fprintf(fp,"\n\tmovb\t%%al, %d(%%rbp)",offr);
					else
						fprintf(fp,"\n\tmovb\t%%al, %s(%%rip)",resx.c_str());
				}
				else
				{
					if(flag1!=0)
						fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off1);
					// Direct Number access
					if(flag2!=0)
					{if(arg2x[0]>='0' && arg2x[0]<='9')
						fprintf(fp,"\n\tmovl\t$%s, %%edx",arg2x.c_str());
					else
						fprintf(fp,"\n\tmovl\t%d(%%rbp), %%edx",off2);}
					fprintf(fp,"\n\tsubl\t%%edx, %%eax");
					if(flag3!=0)
						fprintf(fp,"\n\tmovl\t%%eax, %d(%%rbp)",offr);
					else
						fprintf(fp,"\n\tmovl\t%%eax, %s(%%rip)",resx.c_str());
			
				}
			}
			else if(opx==Q_MULT){
				bool con=search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_char;
				if(con)
				{
					if(flag1!=0)
						fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off1);
					if(flag2!=0)
						fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%edx",off2);
					fprintf(fp,"\n\timull\t%%edx, %%eax");
					if(flag3!=0)
						fprintf(fp,"\n\tmovb\t%%al, %d(%%rbp)",offr);
					else
						fprintf(fp,"\n\tmovb\t%%al, %s(%%rip)",resx.c_str());
				}
				else
				{
					if(flag1!=0)
						fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off1);
					if(flag2!=0)
					{	if(arg2x[0]>='0' && arg2x[0]<='9')
						{
							fprintf(fp,"\n\tmovl\t$%s, %%ecx",arg2x.c_str());
							fprintf(fp,"\n\timull\t%%ecx, %%eax");
						}
					else
						fprintf(fp,"\n\timull\t%d(%%rbp), %%eax",off2);}
					if(flag3!=0)
						fprintf(fp,"\n\tmovl\t%%eax, %d(%%rbp)",offr);
					else
						fprintf(fp,"\n\tmovl\t%%eax, %s(%%rip)",resx.c_str());
				}
			}
	
			else if(opx==Q_DIVIDE){
				if(search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_char)
				{
					if(flag1!=0)
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tcltd");
					if(flag2!=0)
					fprintf(fp,"\n\tidivl\t%d(%%rbp), %%eax",off2);
					else
						fprintf(fp,"\n\tidivl\t%%edx, %%eax");
					if(flag3!=0)
					fprintf(fp,"\n\tmovb\t%%al, %d(%%rbp)",offr);
					else
						fprintf(fp,"\n\tmovb\t%%al, %s(%%rip)",resx.c_str());
				}
				else{
				if(flag1!=0)
				fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off1);
				fprintf(fp,"\n\tcltd");
				if(flag2!=0)
				fprintf(fp,"\n\tidivl\t%d(%%rbp), %%eax",off2);
				else
					fprintf(fp,"\n\tidivl\t%%edx, %%eax");
				if(flag3!=0)
				fprintf(fp,"\n\tmovl\t%%eax, %d(%%rbp)",offr);
				else
					fprintf(fp,"\n\tmovl\t%%eax, %s(%%rip)",resx.c_str());
				}	

			}
			else if(opx==Q_MODULO){
				if(search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax\n\tcltd",off1);
					fprintf(fp,"\n\tidivl\t%d(%%rbp), %%eax",off2);
					fprintf(fp,"\n\tmovl\t%%edx, %%eax\n\tmovb\t%%al, %d(%%rbp)",offr);
				}
				else{
				fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax\n\tcltd",off1);
				fprintf(fp,"\n\tidivl\t%d(%%rbp), %%eax",off2);
				fprintf(fp,"\n\tmovl\t%%edx, %d(%%rbp)",offr);
				}
			}
			else if(opx==Q_UNARY_MINUS){
				if(search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax\n\tnegl\t%%eax",off1);
					fprintf(fp,"\n\tmovb\t%%al, %d(%%rbp)",offr);
				}
				else{
				fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax\n\tnegl\t%%eax",off1);
				fprintf(fp,"\n\tmovl\t%%eax, %d(%%rbp)",offr);
				}
			}
			else if(opx==Q_ASSIGN){
				if(arg1x[0]>='0' && arg1x[0]<='9')
				{
					if(flag1!=0)
					fprintf(fp,"\n\tmovl\t$%s, %d(%%rbp)",arg1x.c_str(),offr);
				}
				else if(arg1x[0] == '\'')
				{
					//Character
					fprintf(fp,"\n\tmovb\t$%d, %d(%%rbp)",(int)arg1x[1],offr);
				}
				else if(flag1 && search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax\n\tmovb\t%%al, %d(%%rbp)",off1,offr);
				}
				else if(flag1&&search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_int)
				{
					if(flag1!=0)
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax\n\tmovl\t%%eax, %d(%%rbp)",off1,offr);
				}
				else if(search(resx)!=NULL && search(resx)->tp_n!=NULL)
				{
					fprintf(fp,"\n\tmovq\t%d(%%rbp), %%rax\n\tmovq\t%%rax, %d(%%rbp)",off1,offr);
				}
				else
				{
					if(flag3!=0)
					{fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax\n\tmovl\t%%eax, %d(%%rbp)",off1,offr);}
					else
					{
						fprintf(fp,"\n\tmovl\t%%eax, %s(%%rip)",resx.c_str());
					}
				}
			}
			else if(opx==Q_PARAM){
				if(resx[0] == '_')
				{
					char* temp = (char*)resx.c_str();
					fprintf(fp,"\n\tmovq\t$.STR%d,\t%%rdi",atoi(temp+1));
				}
				else
				{
					params_stack.push(resx);
					types_stack.push(search(resx)->tp_n->basetp);
					offset_stack.push(offr);
					if(search(resx)->isptrarr==true)
						ptrarr_stack.push(1);
					else
						ptrarr_stack.push(0);
				}
			}

			else if(opx==Q_GOTO){
				if(resx!="-1"&& atoi(resx.c_str())<=end_quad)
					fprintf(fp,"\n\tjmp .L%d",mp_set[atoi(resx.c_str())]);
				else 
					fprintf(fp,"\n\tjmp\t.LRT%d",ret_count);
			}

		
			else if(opx==Q_CALL){
				add_off=function_call(fp);
				fprintf(fp,"\n\tcall\t%s",arg1x.c_str());
				if(resx=="")
				{

				}
				else if(search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_int)
				{
					fprintf(fp,"\n\tmovl\t%%eax, %d(%%rbp)",offr);
				}
				else if(search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovb\t%%al, %d(%%rbp)",offr);
				}
				else if(search(resx)!=NULL && search(resx)->tp_n!=NULL)
				{
					fprintf(fp,"\n\tmovq\t%%rax, %d(%%rbp)",offr);	
				}
				else
				{	
					fprintf(fp,"\n\tmovl\t%%eax, %d(%%rbp)",offr);
				}
				if(arg1x=="printStr")
				{
					fprintf(fp,"\n\taddq $8 , %%rsp");
				}
				else 
				{
					fprintf(fp,"\n\taddq $%d , %%rsp",add_off);
				}
			}
			
			else if(opx==Q_IF_LESS){
				if(search(arg1x)!=NULL && search(arg1x)->tp_n!=NULL&&search(arg1x)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tcmpb\t%d(%%rbp), %%al",off2);
					fprintf(fp,"\n\tjl .L%d",mp_set[atoi(resx.c_str())]);
				}
				else
				{
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%edx",off2);
					fprintf(fp,"\n\tcmpl\t%%edx, %%eax\n\tjl .L%d",mp_set[atoi(resx.c_str())]);
				}
			}
			else if(opx==Q_IF_LESS_OR_EQUAL){
				if(search(arg1x)!=NULL && search(arg1x)->tp_n!=NULL&&search(arg1x)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tcmpb\t%d(%%rbp), %%al",off2);
					fprintf(fp,"\n\tjle .L%d",mp_set[atoi(resx.c_str())]);
				}
				else
				{
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%edx",off2);
					fprintf(fp,"\n\tcmpl\t%%edx, %%eax\n\tjle .L%d",mp_set[atoi(resx.c_str())]);
				}
			}

			else if(opx==Q_IF_GREATER){
				if(search(arg1x)!=NULL && search(arg1x)->tp_n!=NULL&&search(arg1x)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tcmpb\t%d(%%rbp), %%al",off2);
					fprintf(fp,"\n\tjg .L%d",mp_set[atoi(resx.c_str())]);
				}
				else
				{
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%edx",off2);
					fprintf(fp,"\n\tcmpl\t%%edx, %%eax\n\tjg .L%d",mp_set[atoi(resx.c_str())]);
				}
			}
			//case Q_IF_GREATER_OR_EQUAL:
			else if(opx==Q_IF_GREATER_OR_EQUAL){
				if(search(arg1x)!=NULL && search(arg1x)->tp_n!=NULL&&search(arg1x)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tcmpb\t%d(%%rbp), %%al",off2);
					fprintf(fp,"\n\tjge .L%d",mp_set[atoi(resx.c_str())]);
				}
				else
				{
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%edx",off2);
					fprintf(fp,"\n\tcmpl\t%%edx, %%eax\n\tjge .L%d",mp_set[atoi(resx.c_str())]);
				}
			}
			//case Q_IF_EQUAL:
			else if(opx==Q_IF_EQUAL){
				if(search(arg1x)!=NULL && search(arg1x)->tp_n!=NULL&&search(arg1x)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tcmpb\t%d(%%rbp), %%al",off2);
					fprintf(fp,"\n\tje .L%d",mp_set[atoi(resx.c_str())]);
				}
				else
				{
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%edx",off2);
					fprintf(fp,"\n\tcmpl\t%%edx, %%eax\n\tje .L%d",mp_set[atoi(resx.c_str())]);
				}
			}
			else if(opx==Q_IF_NOT_EQUAL){
				if(search(arg1x)!=NULL && search(arg1x)->tp_n!=NULL&&search(arg1x)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tcmpb\t%d(%%rbp), %%al",off2);
					fprintf(fp,"\n\tjne .L%d",mp_set[atoi(resx.c_str())]);
				}
				else
				{
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off1);
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%edx",off2);
					fprintf(fp,"\n\tcmpl\t%%edx, %%eax\n\tjne .L%d",mp_set[atoi(resx.c_str())]);
				}
			}
			//case Q_ADDR:
			else if(opx==Q_ADDR){
				fprintf(fp,"\n\tleaq\t%d(%%rbp), %%rax",off1);
				fprintf(fp,"\n\tmovq\t%%rax, %d(%%rbp)",offr);
			}
			//case Q_LDEREF:
			else if(opx==Q_LDEREF){
				fprintf(fp,"\n\tmovq\t%d(%%rbp), %%rax",offr);
				fprintf(fp,"\n\tmovl\t%d(%%rbp), %%edx\n\tmovl\t%%edx, (%%rax)",off1);
			}
			//case Q_RDEREF:
			else if(opx==Q_RDEREF){
				fprintf(fp,"\n\tmovq\t%d(%%rbp), %%rax",off1);
				fprintf(fp,"\n\tmovl\t(%%rax), %%eax\n\tmovl\t%%eax, %d(%%rbp)",offr);
			}
			//case Q_RINDEX:
			else if(opx==Q_RINDEX){
				// Get Address, subtract offset, get memory
				if(search(arg1x)&&search(arg1x)->isptrarr==true)
				{
					fprintf(fp,"\n\tmovq\t%d(%%rbp), %%rdx",off1);
					fprintf(fp,"\n\tmovslq\t%d(%%rbp), %%rax\n\taddq\t%%rax, %%rdx",off2);
				}
				else
				{
					fprintf(fp,"\n\tleaq\t%d(%%rbp), %%rdx",off1);
					fprintf(fp,"\n\tmovslq\t%d(%%rbp), %%rax\n\taddq\t%%rax, %%rdx",off2);
				}
				if(search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->next&&search(resx)->tp_n->next->basetp == tp_char)
					fprintf(fp,"\n\tmovzbl\t(%%rdx), %%eax\n\tmovb\t%%al, %d(%%rbp)",offr);
				else
					fprintf(fp,"\n\tmovl\t(%%rdx), %%eax\n\tmovl\t%%eax, %d(%%rbp)",offr);
			}
			//case Q_LINDEX:
			else if(opx==Q_LINDEX){
				// Get Address, subtract offset, get memory
				if(search(resx)&&search(resx)->isptrarr==true)
				{
					fprintf(fp,"\n\tmovq\t%d(%%rbp), %%rdx",offr);
					fprintf(fp,"\n\tmovslq\t%d(%%rbp), %%rax",off1);
					fprintf(fp,"\n\taddq\t%%rax, %%rdx");
				}
				else
				{
					fprintf(fp,"\n\tleaq\t%d(%%rbp), %%rdx",offr);
					fprintf(fp,"\n\tmovslq\t%d(%%rbp), %%rax",off1);
					fprintf(fp,"\n\taddq\t%%rax, %%rdx");
				}
				if(search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->next && search(resx)->tp_n->next->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",off2);
					fprintf(fp,"\n\tmovb\t%%al, (%%rdx)");
				}
				else
				{
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",off2);
					fprintf(fp,"\n\tmovl\t%%eax, (%%rdx)");
				}
			}
			//case Q_RETURN:
			else if(opx==Q_RETURN){
				if(resx!="")
				{if(search(resx)!=NULL && search(resx)->tp_n!=NULL&&search(resx)->tp_n->basetp == tp_char)
				{
					fprintf(fp,"\n\tmovzbl\t%d(%%rbp), %%eax",offr);
				}
				else
				{
					fprintf(fp,"\n\tmovl\t%d(%%rbp), %%eax",offr);
				}}
				else
				{
					fprintf(fp,"\n\tmovl\t$0, %%eax");
				}
				fprintf(fp,"\n\tjmp\t.LRT%d",ret_count);
			}
		}
}

