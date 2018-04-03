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

#define YY_EXTRA_TYPE struct lexer_data *

int reset_label_list(label_list** label_list_ptr);
char* remove_spaces(char* source);
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
	
				char* element_name = strdup(yytext);
				yylval->string = remove_spaces(element_name);	
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

int reset_label_list(label_list** label_list_ptr)
{
	label_list_clear(label_list_ptr);
}

// From StackOverflow user Aaron.
char* remove_spaces(char* source)
{
	char* i = source;
  	char* j = source;
  	while(*j != 0)
  	{
    		*i = *j++;
    		if(*i != ' ')
		{
      			i++;
		}
  	}
  		
	*i = 0;
	
	return source;
}	
