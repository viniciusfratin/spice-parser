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

#include <ctype.h>
#include "main.h"
#include "parser.h"
#include "label_list.h"
#include "string_operations.h"

int get_element_type(const char* element_name);
double get_multiplier_value(const char* multiplier_str);
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
					printf("command: %s\n", yytext);
					return TK_COMMAND;
				}	
			}

{element_identifier}	{
			if(yylineno > 1)
			{
				printf("element: %s\n", yytext);

				strcpy(yylval->string, remove_spaces(yytext));	
				return get_element_type(yylval->string);
			}
		}
	
{label} 	{	
			if(yylineno > 1)
			{
				char* label_name = remove_spaces(yytext);
				printf("label: %s\n", label_name);
				strcpy(yylval->string, label_name);
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
				double value;
				char multiplier_str[16];
				sscanf(yytext, "%lf%s", &value, multiplier_str);

				double multiplier_value = get_multiplier_value(multiplier_str);
				// Invalid multiplier.
				if(multiplier_value == -1)
				{
					printf("bad multiplier '%s' at line %d\n", multiplier_str, yylineno);
					exit(1);
				}

				value = value * multiplier_value;
				printf("mult numeric value: %le\n", value);

				element_value elem_val;
				elem_val.value.numeric_value = value;
				elem_val.is_numeric = 1;

				yylval->value = elem_val;

				return TK_VALUE;
			}
		}




{text_value}	{
			if(yylineno > 1)
			{
				element_value elem_val;
				strcpy(elem_val.value.string_value, yytext);
				elem_val.is_numeric = 0;

				yylval->value = elem_val;
				
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

int get_element_type(const char* element_name)
{
	char element_identifier = tolower(element_name[0]);
	switch(element_identifier)
	{
		case 'r':
			return TK_RESISTOR;
			break;

		case 'c':
			return TK_CAPACITOR;
			break;

		case 'l':
			return TK_INDUCTOR;
			break;
	
		case 'v':
			return TK_V_SOURCE;
			break;

		case 'i':
			return TK_C_SOURCE;
			break;

		case 'e':
			return TK_VCV_SOURCE;
			break;

		case 'f':
			return TK_CCC_SOURCE;
			break;

		case 'g':
			return TK_VCC_SOURCE;
			break;

		case 'h':
			return TK_CCV_SOURCE;
			break;

		case 'd':
			return TK_DIODE;
			break;

		case 'q':
			return TK_TJB;
			break;

		case 'm':
			return TK_MOSFET;
			break;

		default:
			return TK_INVALID_ELEMENT;
			break;
	}
}

double get_multiplier_value(const char* multiplier_str)
{
	char lowercase_multiplier[16];
	strcpy(lowercase_multiplier, multiplier_str);
	convert_to_lowercase(lowercase_multiplier);
	
	if(strcmp(lowercase_multiplier, "f") == 0)
	{
		return 1e-15;
	}

	else if(strcmp(lowercase_multiplier, "p") == 0)
	{
		return 1e-12;
	}

	else if(strcmp(lowercase_multiplier, "n") == 0)
	{
		return 1e-9;
	}

	else if(strcmp(lowercase_multiplier, "u") == 0)
	{
		return 1e-6;
	}

	else if(strcmp(lowercase_multiplier, "m") == 0)
	{
		return 1e-3;
	}

	else if(strcmp(lowercase_multiplier, "k") == 0)
	{
		return 1e3;
	}
	
	else if(strcmp(lowercase_multiplier, "meg") == 0)
	{
		return 1e6;
	}

	else if(strcmp(lowercase_multiplier, "g") == 0)
	{
		return 1e9;
	}

	else if(strcmp(lowercase_multiplier, "t") == 0)
	{
		return 1e12;
	}

	return -1;
}

