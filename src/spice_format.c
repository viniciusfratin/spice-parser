#include <ctype.h>
#include <string.h>
#include "spice_format.h"
#include "string_operations.h"

int get_element_type(const char* element_name)
{
	char element_identifier = tolower(element_name[0]);
	switch(element_identifier)
	{
		case 'r':
			return TYPE_RESISTOR;
			break;

		case 'c':
			return TYPE_CAPACITOR;
			break;

		case 'l':
			return TYPE_INDUCTOR;
			break;
	
		case 'v':
			return TYPE_V_SOURCE;
			break;

		case 'i':
			return TYPE_C_SOURCE;
			break;

		case 'e':
			return TYPE_VCV_SOURCE;
			break;

		case 'f':
			return TYPE_CCC_SOURCE;
			break;

		case 'g':
			return TYPE_VCC_SOURCE;
			break;

		case 'h':
			return TYPE_CCV_SOURCE;
			break;

		case 'd':
			return TYPE_DIODE;
			break;

		case 'q':
			return TYPE_TJB;
			break;

		case 'm':
			return TYPE_MOSFET;
			break;

		default:
			return TYPE_INVALID_ELEMENT;
			break;
	}
}

void get_element_type_name(char* ret_str, int type)
{
	switch(type)
	{
		case TYPE_RESISTOR:
			strcpy(ret_str, "RESISTOR");
			break;

		case TYPE_CAPACITOR:
			strcpy(ret_str, "CAPACITOR");
			break;
	
		case TYPE_INDUCTOR:
			strcpy(ret_str, "INDUCTOR");
			break;

		case TYPE_V_SOURCE:
			strcpy(ret_str, "V_SOURCE");
			break;

		case TYPE_C_SOURCE:
			strcpy(ret_str, "C_SOURCE");
			break;

		case TYPE_VCV_SOURCE:
			strcpy(ret_str, "VCV_SOURCE");
			break;
	
		case TYPE_CCC_SOURCE:
			strcpy(ret_str, "CCC_SOURCE");
			break;

		case TYPE_VCC_SOURCE:
			strcpy(ret_str, "VCC_SOURCE");
			break;

		case TYPE_CCV_SOURCE:
			strcpy(ret_str, "CCV_SOURCE");
			break;

		case TYPE_DIODE:
			strcpy(ret_str, "DIODE");
			break;

		case TYPE_TJB:
			strcpy(ret_str, "TJB");
			break;
		
		case TYPE_MOSFET:
			strcpy(ret_str, "MOSFET");
			break;

		default:
			strcpy(ret_str, "INVALID_ELEMENT");
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

