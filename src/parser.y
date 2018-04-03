%define parse.error verbose
%define api.pure true
%locations
%token-table
%glr-parser
%lex-param {void *scanner}
%parse-param {void *scanner}

%{

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lexer.h"

int yyerror(YYLTYPE *locp, yyscan_t scanner, const char *msg);

%}

%union {
	int value;
}

%token TK_WHITESPACE
%token TK_NEW_LINE
%token TK_TEXT
%token TK_COMMAND
%token TK_LABEL
%token TK_PARAMETERS
%token TK_VALUE
%token TK_RESISTOR
%token TK_CAPACITOR
%token TK_INDUCTOR
%token TK_V_SOURCE
%token TK_C_SOURCE
%token TK_VCV_SOURCE
%token TK_CCC_SOURCE
%token TK_VCC_SOURCE
%token TK_CCV_SOURCE
%token TK_DIODE
%token TK_TJB
%token TK_MOSFET
%token TK_ELEMENT

%%

spice: 
        first_line body
        ;

first_line:	
		TK_NEW_LINE 
		;
		
body:
		%empty
		| TK_NEW_LINE body
		| command body
		| element body
		;

command:
		command_identifier command_parameters
		;

command_identifier:
		TK_COMMAND
			{
				printf("command\n");
			}
		;

command_parameters:
		%empty
		| TK_LABEL command_parameters
		| TK_VALUE command_parameters
		
element:
		element_identifier element_label element_value
		;
		
element_identifier:
		TK_ELEMENT
			{
				printf("element\n");
			}
		;
		
element_label:
		%empty
		| TK_LABEL element_label
			{
				printf("label\n");
			}
		;

element_value:
		TK_VALUE
			{
				printf("value\n");
			}
		;
		
%%

int yyerror(YYLTYPE *locp, yyscan_t scanner, const char *msg) 
{
	if (locp) 
	{
   		fprintf(stderr, "parse error: %s (:%d.%d -> :%d.%d)\n",
                    msg,
                    locp->first_line, locp->first_column,
                    locp->last_line,  locp->last_column
    		);
  	} 
	
	else 
	{
    		fprintf(stderr, "parse error: %s\n", msg);
  	}
  	
	return 0;
}
