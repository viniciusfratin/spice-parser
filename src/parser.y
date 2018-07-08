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
#include "parameter_list.h"
#include "parser.h"
#include "lexer.h"

int yyerror(YYLTYPE *locp, yyscan_t scanner, struct parser_data *p_data, const char *msg);


%}

%union {
	char string[512];
	double number;
	element_value value;
	number_desc val_type;
	special_cc cc;
	parameter param;
	parameter_list* param_list;
}

%token TK_NEW_LINE
%token <string> TK_COMMAND
%token <string> TK_LABEL
%token <val_type> TK_NUMBER
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
%type <cc> three_node_element
%type <string> four_node_element

%type <string> node;
%type <value> value

%type <param_list> param_list
%type <param> param

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

				generic_list_insert(p_data->command_list, (void*)&cmd);
			}
		;

command_parameters:
		%empty
		| TK_LABEL command_parameters
		| TK_NUMBER command_parameters
		
element: 
		two_node_element node node value param_list
			{
				label_list_insert(p_data->label_list, $2);
				label_list_insert(p_data->label_list, $3);

				label *l1, *l2;
				label_list_contains_name(*p_data->label_list, $2, &l1);
				label_list_contains_name(*p_data->label_list, $3, &l2);
				
				element elem;
					
				elem.type = get_element_type($1);
				strcpy(elem.name, $1);
				elem.is_set = 1;
				generic_list_initialize(&elem.nodes);
				generic_list_insert(&elem.nodes, (void*)l1);
				generic_list_insert(&elem.nodes, (void*)l2);
				elem.value = $4;
				elem.param_list = $5;
				elem.companion_elements = NULL;

				element_list_insert(p_data->element_list, elem);
			}
		| three_node_element node node node value param_list
			{
				special_cc c = $1;

				label_list_insert(p_data->label_list, $2);
				label_list_insert(p_data->label_list, $3);
			
				element* element_ptr;
				element_list_contains_name(*p_data->element_list, $4, &element_ptr);
				
				int is_elem = !!element_ptr;
				if(!is_elem)
				{	
					if(c.is_cc)
					{
						printf("CC! %s\n", c.string);
						element dummy_elem;
					
						strcpy(dummy_elem.name, $4);
						dummy_elem.is_set = 0;
						element_list_insert(p_data->element_list, dummy_elem);
						element_list_contains_name(*p_data->element_list, $4, &element_ptr);
					}

					else
					{
						label_list_insert(p_data->label_list, $4);
					}
				}

				label *l1, *l2, *l3;
				label_list_contains_name(*p_data->label_list, $2, &l1);
				label_list_contains_name(*p_data->label_list, $3, &l2);

				if(!is_elem && !c.is_cc)
				{
					label_list_contains_name(*p_data->label_list, $4, &l3);
				}

				element elem;
					
				elem.type = get_element_type(c.string);
				strcpy(elem.name, c.string);
				elem.is_set = 1;
				generic_list_initialize(&elem.nodes);
				generic_list_insert(&elem.nodes, (void*)l1);
				generic_list_insert(&elem.nodes, (void*)l2);
					
				if(!is_elem && !c.is_cc)
				{
					generic_list_insert(&elem.nodes, (void*)l3);
				}

				else
				{
					generic_list_initialize(&elem.ref_elements);
					generic_list_insert(&elem.ref_elements, (void*)element_ptr);
				}

				elem.value = $5;
				elem.param_list = $6;
				elem.companion_elements = NULL;

				element_list_insert(p_data->element_list, elem);

			}
		| four_node_element node node node node value param_list
			{
				label_list_insert(p_data->label_list, $2);
				label_list_insert(p_data->label_list, $3);
				label_list_insert(p_data->label_list, $4);
				label_list_insert(p_data->label_list, $5);

				label *l1, *l2, *l3, *l4;
				label_list_contains_name(*p_data->label_list, $2, &l1);
				label_list_contains_name(*p_data->label_list, $3, &l2);
				label_list_contains_name(*p_data->label_list, $4, &l3);
				label_list_contains_name(*p_data->label_list, $5, &l4);
			
				element elem;
					
				elem.type = get_element_type($1);
				strcpy(elem.name, $1);
				elem.is_set = 1;
				generic_list_initialize(&elem.nodes);
				generic_list_insert(&elem.nodes, (void*)l1);
				generic_list_insert(&elem.nodes, (void*)l2);
				generic_list_insert(&elem.nodes, (void*)l3);
				generic_list_insert(&elem.nodes, (void*)l4);
				elem.value = $6;
				elem.param_list = $7;
				elem.companion_elements = NULL;

				element_list_insert(p_data->element_list, elem);
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
		TK_TJB 		
			{ 
				special_cc c;
				c.is_cc = 0;
				strcpy(c.string, $1);
				$$ = c; 
			}

		| TK_MOSFET 	
			{ 
				special_cc c;
				c.is_cc = 0;
				strcpy(c.string, $1); 
				$$ = c;
			}

		| TK_CCV_SOURCE 
			{ 
				special_cc c;
				c.is_cc = 1;
				strcpy(c.string, $1); 
				$$ = c;
			}

		| TK_CCC_SOURCE 
			{ 
				special_cc c;
				c.is_cc = 1;
				strcpy(c.string, $1); 
				$$ = c;
			}
	;

four_node_element:
		TK_VCV_SOURCE	{ strcpy($$, $1); }
		| TK_VCC_SOURCE { strcpy($$, $1); }
	
node:
		TK_LABEL
			{
				strcpy($$, $1);
			}

		| TK_NUMBER
			{
				strcpy($$, $1.string);
			}

value:
		TK_LABEL
			{
				element_value value;
				value.is_numeric = 0;
				strcpy(value.value.string_value, $1);
				
				$$ = value;
			}

		| TK_NUMBER
			{
				element_value value;
				value.is_numeric = 1;
				value.value.numeric_value = $1.number;

				$$ = value;
			}

	
param_list:
		%empty
			{
				$$ = NULL;
			}

		| param_list param
			{
				parameter_list* param_list = $1;
				parameter_list_insert(&param_list, $2);

				$$ = param_list;
			}

param:
		TK_LABEL '=' TK_NUMBER
			{
				parameter param;
				strcpy(param.name, $1);
				param.value = $3.number;

				$$ = param;
			}

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
