%{

#include <stdio.h>
#include <stdlib.h>
int yylex(void);
int yyerror(const char *s);

%}

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

%%

spice: 
        first_line body
        ;

first_line:	
		TK_NEW_LINE 
		;
		
body:
		command
		| element {printf("blergh");}
		;

command:
		TK_COMMAND
		;
		
element:
		element_identifier element_label
		;
		
element_identifier:
		TK_RESISTOR
			{
				printf("resistor\n");
			}
		;
		
element_label:
		TK_LABEL
			{
				printf("label\n");
			}
		;
	
%%