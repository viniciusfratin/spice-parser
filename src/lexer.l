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
#include "spice_format.h"
#include "parameter_list.h"
#include "parser.h"
#include "label_list.h"
#include "string_operations.h"

int get_element_token_type(const char* element_name);
double get_multiplier_value(const char* multiplier_str);


%}

misc_symbol ,|;|:|\(|\)|\[|\]|\{|\}|\+|-|\*|\/|<|>|!|&|\$|%|#|\^ 
common_text ({misc_symbol}|[a-zA-Z0-9_])+
whitespace [ \t]+
new_line "\n"
multiplier "f"|"p"|"n"|"u"|"m"|"k"|"meg"|"g"|"t"
whole_number (-)?([0-9])+
simple_number {whole_number}|({whole_number}"."{whole_number})
sci_number {simple_number}"E"{simple_number}
mult_number {simple_number}{multiplier}
simple_number_value {simple_number}
sci_number_value {sci_number}
mult_number_value {mult_number}
comment "*"(.*)
identifier ^(.{common_text})
label {common_text}
equal_sign =

%%

{new_line}	{
			return TK_NEW_LINE;
		}

{comment}	{ }

{whitespace}	{ }

{equal_sign}	{
			return *yytext;
		}

{identifier}	{
			if(yylineno > 1)
			{
				strcpy(yylval->string, remove_spaces(yytext));	
				if(get_element_token_type(yylval->string) != TK_INVALID_ELEMENT)
				{
					return get_element_token_type(yylval->string);
				}

				else
				{
					return TK_COMMAND;
				}
			}
		}
	

{simple_number_value} {
			if(yylineno > 1)
			{
				double value;
				sscanf(yytext, "%lf", &value);
				
				number_desc val_type;
				val_type.number = value;
				strcpy(val_type.string, remove_spaces(yytext));
				yylval->val_type = val_type;

				return TK_NUMBER;
			}
		}

{sci_number_value} {
			if(yylineno > 1)
			{
				double value;
				sscanf(yytext, "%le", &value);

				number_desc val_type;
				val_type.number = value;
				strcpy(val_type.string, remove_spaces(yytext));	
				yylval->val_type = val_type;

				return TK_NUMBER;
			}
		}

{mult_number_value} {
			if(yylineno > 1)
			{
				double value;
				char multiplier_str[16];
				sscanf(yytext, "%lf%s", &value, multiplier_str);

				double multiplier_value = get_multiplier_value(multiplier_str);
				
				value = value * multiplier_value;

				number_desc val_type;
				val_type.number = value;
				strcpy(val_type.string, remove_spaces(yytext));

				yylval->val_type = val_type;

				return TK_NUMBER;
			}
		}

{label} 	{	
			if(yylineno > 1)
			{
				char* label_name = remove_spaces(yytext);
				strcpy(yylval->string, label_name);
				return TK_LABEL;
			}	
		}			

.	{
		if(yylineno > 1)
		{
			fprintf(stderr, "Bad input '%s' at line %d\n", yytext, yylineno);
			exit(1);
		}
	}
%%

int get_element_token_type(const char* element_name)
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
