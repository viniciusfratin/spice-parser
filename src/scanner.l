%{

#include "y.tab.h"

#define BOOL char
#define TRUE 1
#define FALSE 0

int yyparse();
int yyerror(char *errormsg);
BOOL must_register();

int line_counter = 1;

%}

%option caseless

whitespace [ \t]+
new_line [\n]
label .*
multiplier "f"|"p"|"n"|"u"|"m"|"k"|"meg"|"g"|"t"
simple_number ([0-9])+"."([0-9])+
sci_number (simple_number)("E")(simple_number)
mult_number (simple_number)(multiplier)
number (simple_number)|(sci_number)|(mult_number)
value (number)|(text)
comment "*"

%%

{whitespace}	{
					if(must_register())
					{
						printf("whitespace\n");
						return TK_WHITESPACE;
					}
				}

{new_line}		{
					line_counter++;
					printf("new line\n");
					return TK_NEW_LINE;
				}
				
{comment}		{printf("comment\n"); }

{value}			{
					if(must_register())
					{
						printf("value\n");
						return TK_VALUE;
					}
				}
				
R				{
					if(must_register())
					{
						printf("resistor\n");
						return TK_RESISTOR;
					}
				}
				
..			{
					if(must_register())
					{
						printf("label\n");
						return TK_LABEL;
					}
				}

%%

int main(void)
{
   yyparse();
   return 0;
}

int yywrap(void)
{
   return 0;
}

int yyerror(char *errormsg)
{
    fprintf(stderr, "%s\n", errormsg);
    exit(1);
}

BOOL must_register()
{
	if(line_counter == 1)
	{
		return FALSE;
	}
	
	else
	{
		return TRUE;
	}
}