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
#include "label_list.h"
#include "string_operations.h"

#define YY_EXTRA_TYPE struct lexer_data *

double get_multiplier_value(const char* multiplier_str);
int reset_label_list(label_list** label_list_ptr);
%}

common_text [a-zA-Z0-9_]+
whitespace [ \t]+
new_line "\n"
multiplier "f"|"p"|"n"|"u"|"m"|"k"|"meg"|"g"|"t"
whole_number ([0-9])+ 
simple_number {whole_number}|({whole_number}"."{whole_number})
sci_number {simple_number}"E"{simple_number}
mult_number {simple_number}{multiplier}
simple_number_value {simple_number}$
sci_number_value {sci_number}$
mult_number_value {mult_number}$
comment "*"(.*)
command_identifier ^("."{common_text}){whitespace}
element_identifier ^(.{common_text}){whitespace}
label {common_text}{whitespace}
text_value {common_text}$

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
					reset_label_list(&yyextra->current_label_list);
					printf("command: %s\n", yytext);
					return TK_COMMAND;
				}	
			}

{element_identifier}	{
			if(yylineno > 1)
			{
				reset_label_list(&yyextra->current_label_list);
				printf("element: %s\n", yytext);

				strcpy(yylval->string, remove_spaces(yytext));	
				return TK_ELEMENT;
			}
		}
	
{label} 	{	
			if(yylineno > 1)
			{
				label_list_insert(&yyextra->global_label_list, remove_spaces(yytext));
				label_list_insert(&yyextra->current_label_list, remove_spaces(yytext));
			
				/*label_list* current_element = yyextra->global_label_list;
				while(current_element != NULL)
				{
					label* l = (label*)(current_element->value);
					printf("global list elem: %d %s\n", l->id, l->name);
					current_element = current_element->next;
				}*/

				printf("label: %s\n", remove_spaces(yytext));
				yylval->l_list = yyextra->current_label_list;
				return TK_LABEL;
			}	
		}			

{simple_number_value} {
			if(yylineno > 1)
			{
				double value;
				sscanf(yytext, "%lf", &value);
				
				element_value elem_val;
				elem_val.value.numeric_value = value;
				elem_val.is_numeric = 1;

				printf("simple numeric value: %lf\n", value);
				yylval->value = elem_val;
				return TK_VALUE;
			}
		}

{sci_number_value} {
			if(yylineno > 1)
			{
				double value;
				sscanf(yytext, "%le", &value);
	
				element_value elem_val;
				elem_val.value.numeric_value = value;
				elem_val.is_numeric = 1;

				printf("sci numeric value: %s\n", yytext);
				yylval->value = elem_val;
				return TK_VALUE;
			}
		}

{mult_number_value} {
			if(yylineno > 1)
			{
				char multiplier[5];
				sscanf(yytext, "%*f%s", multiplier);
				convert_to_lowercase(multiplier);
				printf("mult numeric value: %s; multipler: %s\n", yytext, multiplier);
				return TK_VALUE;
			}
		}




{text_value}	{
			if(yylineno > 1)
			{
				printf("text value: %s\n", yytext);
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


double get_multiplier_value(const char* multiplier_str)
{
	if(strcmp(multiplier_str, "f") == 0)
	{
		return 1e-15;
	}

	else if(strcmp(multiplier_str, "p") == 0)
	{
		return 1e-12;
	}

	else if(strcmp(multiplier_str, "n") == 0)
	{
		return 1e-9;
	}

	else if(strcmp(multiplier_str, "u") == 0)
	{
		return 1e-6;
	}

	else if(strcmp(multiplier_str, "m") == 0)
	{
		return 1e-3;
	}

	else if(strcmp(multiplier_str, "k") == 0)
	{
		return 1e3;
	}
	
	else if(strcmp(multiplier_str, "meg") == 0)
	{
		return 1e6;
	}

	else if(strcmp(multiplier_str, "g") == 0)
	{
		return 1e9;
	}

	else if(strcmp(multiplier_str, "t") == 0)
	{
		return 1e12;
	}

	return 0;
}

int reset_label_list(label_list** label_list_ptr)
{
	label_list_clear(label_list_ptr);
}	
