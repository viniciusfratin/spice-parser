%option bison-bridge
%option bison-locations
%option ecs
%option nodefault
%option noyywrap
%option reentrant
%option stack
%option warn
%option yylineno
%option caseless

%{

#include "main.h"
#include "parser.h"

#define YY_EXTRA_TYPE struct lexer_data *

%}

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
					{
						printf("whitespace\n");
						return TK_WHITESPACE;
					}
				}

{new_line}		{
					yyextra->line_number++;
					printf("new line %d\n", yyextra->line_number);
					return TK_NEW_LINE;
				}
				
{comment}		{ printf("comment\n"); }

{value}			{
					{
						printf("value\n");
						return TK_VALUE;
					}
				}
				
R				{
					{
						printf("resistor\n");
						return TK_RESISTOR;
					}
				}
				
..			{
					{
						printf("label\n");
						return TK_LABEL;
					}
				}

%%


