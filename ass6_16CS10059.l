%{
	#include "ass6_16CS10059_translator.h"
	#include "y.tab.h"
	#include<stdio.h>
	extern void yyerror(const char *);
	static void comment(void);
%}

/*Identifier*/
IDENTIFIER 		[_a-zA-Z][_a-zA-Z0-9]*

/*Constants*/
INTEGER_CONSTANT	[1-9][0-9]*|0

FRACTIONAL_CONSTANT	[.][0-9]+|[0-9]+[.][0-9]+|[0-9]+[.]
EXPONENT_PART		[eE][+\-][0-9]+|[eE][0-9]+
FLOATING_CONSTANT		{FRACTIONAL_CONSTANT}|{FRACTIONAL_CONSTANT}{EXPONENT_PART}|[0-9]+{EXPONENT_PART}
ENUMERATION_CONSTANT	{IDENTIFIER}

/*String Literal and Character Sequence*/
ESC_SEQ				[\\][']|[\\]["]|[\\][?]|[\\][\\]|[\\][a]|[\\][b]|[\\][f]|[\\][n]|[\\][r]|[\\][t]|[\\][v]
C_CHAR				[^'\n\\]|{ESC_SEQ}
CHAR_SEQUENCE		{C_CHAR}+
CHARACTER_CONSTANT		[']{CHAR_SEQUENCE}[']
S_CHAR				[^"\n\\]|{ESC_SEQ}
STRING_LITERAL      ["]{S_CHAR}*["]

/*Comments and white spaces*/
SINGLELINE_COMMENT 	"//".*
MULTILINE_COMMENT	"/*"([^*]|\*+[^*/])*\*+"/"
WS 		[ \t\n]
COMMENT 	{SINGLELINE_COMMENT}|{MULTILINE_COMMENT}

%%

"auto"              {return AUTO;}
"enum"              {return ENUM;}
"restrict"          {return RESTRICT;} 
"unsigned"          {return UNSIGNED;}
"break"             {return BREAK;}
"extern"            {return EXTERN;}
"return"            {return RETURN;}
"void"              {return VOID;}
"char"              {return CHAR;}
"for"              	{return FOR;}
"signed"            {return SIGNED;}
"while"             {return WHILE;}
"const"             {return CONST;}
"goto"              {return GOTO;}
"sizeof"            {return SIZEOF;}
"case"              {return CASE;}
"float"             {return FLOAT;}
"short"             {return SHORT;}
"volatile"          {return VOLATILE;}
"_Bool"             {return BOOL;}
"continue"          {return CONTINUE;}
"if"                {return IF;}
"static"            {return STATIC;}
"_Complex"          {return COMPLEX;}
"default"           {return DEFAULT;}
"inline"            {return INLINE;}
"struct"            {return STRUCT;}
"_Imaginary"        {return IMAGINARY;}
"do"                {return DO;}
"int"               {return INT;}
"switch"            {return SWITCH;}
"double"            {return DOUBLE;}
"long"              {return LONG;}
"typedef"           {return TYPEDEF;}
"else"              {return ELSE;}
"register"          {return REGISTER;}
"union"             {return UNION;}


"["                 {return '[';  }
"]"                 { return ']'; }
"("                 { return '('; }
")"                 { return ')'; }
"{"                 { return '{'; }
"}"                 { return '}'; }
"."                 { return '.'; }
"->"                { return (POINTER); }
"++"                { return (INCREMENT); }
"--"                { return (DECREMENT); }
"&"                 { return '&'; }
"*"                 { return '*'; }
"+"                 { return '+'; }
"-"                 { return '-'; }
"~"                 { return '~'; }
"!"                 { return '!'; }
"/"                 { return '/'; }
"%"                 { return '%'; }
"<<"                { return (LEFT_SHIFT); }
">>"                { return (RIGHT_SHIFT); }
"<"                 { return '<'; }
">"                 { return '>'; }
"<="                { return (LESS_EQUALS); }
">="                { return (GREATER_EQUALS); }
"=="                { return (EQUALS); }
"!="                { return (NOT_EQUALS); }
"^"                 { return '^'; }
"|"                 { return '|'; }
"&&"                { return (AND); }
"||"                { return (OR); }
"?"                 { return '?'; }
":"                 { return ':'; }
";"                 { return ';'; }
"..."               { return (ELLIPSIS); }
"="                 { return '='; }
"*="                { return MULTIPLY_ASSIGN; }
"/="                { return DIVIDE_ASSIGN; }
"%="                { return MODULO_ASSIGN; }
"+="                { return ADD_ASSIGN; }
"-="                { return SUBTRACT_ASSIGN; }
"<<="               { return LEFT_SHIFT_ASSIGN; }
">>="               { return RIGHT_SHIFT_ASSIGN; }
"&="                { return AND_ASSIGN; }
"^="                { return XOR_ASSIGN; }
"|="                { return OR_ASSIGN; }
","                 { return ','; }
"#"                 { return '#'; }


{IDENTIFIER} { 
	yylval.idl.name=new string(yytext);
	return IDENTIFIER;
}
{INTEGER_CONSTANT} { 
	yylval.intval=atoi(yytext); 
	return INTEGER_CONSTANT;
}
{FLOATING_CONSTANT} { 
	yylval.floatval=atof(yytext); 
	return FLOATING_CONSTANT;
}
{ENUMERATION_CONSTANT} { 
	return(ENUMERATION_CONSTANT);
}
{CHARACTER_CONSTANT} { 
	yylval.charval=yytext[1]; return CHAR_CONST;
}
{STRING_LITERAL} { 
	yylval.strval = new string(yytext);
	return STRING_LITERAL;
}

{COMMENT}	{}
{WS}        {}

%%
