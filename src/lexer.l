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

common_text [a-zA-Z0-9_]+
whitespace [ \t]+
new_line "\n"
multiplier "f"|"p"|"n"|"u"|"m"|"k"|"meg"|"g"|"t"
whole_number ([0-9])+ 
simple_number {whole_number}|({whole_number}"."{whole_number})
sci_number {simple_number}"E"{simple_number}
mult_number {simple_number}{multiplier}
number {simple_number}|{sci_number}|{mult_number}
comment "*"(.*)
command_identifier ^("."{common_text}){whitespace}
element_identifier ^(.{common_text}){whitespace}
label {common_text}{whitespace}
value {number}|{common_text}$

%%

{whitespace} 	{
			if(yylineno > 1)
			{
				//printf("whitespace\n");
				return TK_WHITESPACE;
			}
		}

{new_line}	{
			//printf("new line %d\n", yylineno);
			return TK_NEW_LINE;
		}


{comment}	{
			printf("comment: %s\n", yytext);
		}
						
{command_identifier}	{
				if(yylineno > 1)
				{
					printf("command: %s\n", yytext);
					return TK_COMMAND;
				}	
			}

{element_identifier}	{
			if(yylineno > 1)
			{
				printf("element: %s\n", yytext);
				return TK_ELEMENT;
			}
		}
	
{label} 	{	
			if(yylineno > 1)
			{
				printf("label: %s\n", yytext);
				return TK_LABEL;
			}	
		}			

{value} 	{
			if(yylineno > 1)
			{
				printf("value: %s\n", yytext);
				return TK_VALUE;
			}
		}

.	{
		if(yylineno > 1)
		{
			printf("bad input character '%s' at line %d\n", yytext, yylineno);
			exit(1);
		}
	}
%%


