%{

#include "y.tab.h"

%}

%option reentrant
%option bison-bridge
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
				
{comment}		{ printf("comment\n"); }

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


