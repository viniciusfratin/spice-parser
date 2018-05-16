%define parse.error verbose
%define api.pure true
%locations
%token-table
%glr-parser
%lex-param {void *scanner}
%parse-param {void *scanner} {struct parser_data *p_data}

%{

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "label_list.h"
#include "spice_format.h"
#include "parser.h"
#include "lexer.h"

int yyerror(YYLTYPE *locp, yyscan_t scanner, struct parser_data *p_data, const char *msg);

%}

%union {
	char string[512];
	element_value value;
}

%token TK_NEW_LINE
%token <string> TK_COMMAND
%token <string> TK_LABEL
%token <value> TK_VALUE
%token <string> TK_RESISTOR
%token <string> TK_CAPACITOR
%token <string> TK_INDUCTOR
%token <string> TK_V_SOURCE
%token <string> TK_C_SOURCE
%token <string> TK_VCV_SOURCE
%token <string> TK_CCC_SOURCE
%token <string> TK_VCC_SOURCE
%token <string> TK_CCV_SOURCE
%token <string> TK_DIODE
%token <string> TK_TJB
%token <string> TK_MOSFET
%token <string> TK_INVALID_ELEMENT

%type <string> two_node_element
%type <string> three_node_element
%type <string> four_node_element

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
				int command_type = get_command_type($1);

				command cmd;
				cmd.type = command_type;
				generic_list_initialize(&cmd.parameters);

				generic_list_insert(&p_data->command_list, (void*)&cmd);
			}
		;

command_parameters:
		%empty
		| TK_LABEL command_parameters
		| TK_VALUE command_parameters
		
element: 
		two_node_element TK_LABEL TK_LABEL TK_VALUE
			{
				label_list_insert(&p_data->label_list, $2);
				label_list_insert(&p_data->label_list, $3);

				label *l1, *l2;
				label_list_contains_name(p_data->label_list, $2, &l1);
				label_list_contains_name(p_data->label_list, $3, &l2);
				
				element elem;
					
				elem.type = get_element_type($1);
				strcpy(elem.name, $1);
				generic_list_initialize(&elem.nodes);
				generic_list_insert(&elem.nodes, (void*)l1);
				generic_list_insert(&elem.nodes, (void*)l2);
				elem.value = $4;

				element_list_insert(&p_data->element_list, elem);
			}
		| three_node_element TK_LABEL TK_LABEL TK_LABEL TK_VALUE
			{
				label_list_insert(&p_data->label_list, $2);
				label_list_insert(&p_data->label_list, $3);
				label_list_insert(&p_data->label_list, $4);

				label *l1, *l2, *l3;
				label_list_contains_name(p_data->label_list, $2, &l1);
				label_list_contains_name(p_data->label_list, $3, &l2);
				label_list_contains_name(p_data->label_list, $4, &l3);
			
				element elem;
					
				elem.type = get_element_type($1);
				strcpy(elem.name, $1);
				generic_list_initialize(&elem.nodes);
				generic_list_insert(&elem.nodes, (void*)l1);
				generic_list_insert(&elem.nodes, (void*)l2);
				generic_list_insert(&elem.nodes, (void*)l3);
				elem.value = $5;

				element_list_insert(&p_data->element_list, elem);

			}
		| four_node_element TK_LABEL TK_LABEL TK_LABEL TK_LABEL TK_VALUE
			{
				label_list_insert(&p_data->label_list, $2);
				label_list_insert(&p_data->label_list, $3);
				label_list_insert(&p_data->label_list, $4);
				label_list_insert(&p_data->label_list, $5);

				label *l1, *l2, *l3, *l4;
				label_list_contains_name(p_data->label_list, $2, &l1);
				label_list_contains_name(p_data->label_list, $3, &l2);
				label_list_contains_name(p_data->label_list, $4, &l3);
				label_list_contains_name(p_data->label_list, $5, &l4);
			
				element elem;
					
				elem.type = get_element_type($1);
				strcpy(elem.name, $1);
				generic_list_initialize(&elem.nodes);
				generic_list_insert(&elem.nodes, (void*)l1);
				generic_list_insert(&elem.nodes, (void*)l2);
				generic_list_insert(&elem.nodes, (void*)l3);
				generic_list_insert(&elem.nodes, (void*)l4);
				elem.value = $6;

				element_list_insert(&p_data->element_list, elem);
			}
		;

two_node_element:
		TK_RESISTOR 	{ strcpy($$, $1); }
		| TK_CAPACITOR 	{ strcpy($$, $1); }
		| TK_INDUCTOR 	{ strcpy($$, $1); }
		| TK_V_SOURCE 	{ strcpy($$, $1); }
		| TK_C_SOURCE 	{ strcpy($$, $1); }
		| TK_DIODE 	{ strcpy($$, $1); }
	;

three_node_element:
		TK_TJB 		{ strcpy($$, $1); }
		| TK_MOSFET 	{ strcpy($$, $1); }
	;

four_node_element:
		TK_VCV_SOURCE	{ strcpy($$, $1); }
		| TK_CCC_SOURCE { strcpy($$, $1); }
		| TK_VCC_SOURCE { strcpy($$, $1); }
		| TK_CCV_SOURCE { strcpy($$, $1); }
		
%%

int yyerror(YYLTYPE *locp, yyscan_t scanner, struct parser_data *p_data, const char *msg) 
{
	if (locp) 
	{
   		fprintf(stderr, "Parse error: %s (:%d.%d -> :%d.%d)\n",
                    msg,
                    locp->first_line, locp->first_column,
                    locp->last_line,  locp->last_column
    		);
  	} 
	
	else 
	{
    		fprintf(stderr, "Parse error: %s\n", msg);
  	}
  	
	return 0;
}
