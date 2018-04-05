#ifndef SPICE_FORMAT_H_
#define SPICE_FORMAT_H_

#include "label_list.h"

#define TYPE_RESISTOR 1
#define TYPE_CAPACITOR 2
#define TYPE_INDUCTOR 3
#define TYPE_V_SOURCE 4
#define TYPE_C_SOURCE 5
#define TYPE_VCV_SOURCE 6
#define TYPE_CCC_SOURCE 7
#define TYPE_VCC_SOURCE 8
#define TYPE_CCV_SOURCE 9
#define TYPE_DIODE 10
#define TYPE_TJB 11
#define TYPE_MOSFET 12
#define TYPE_INVALID_ELEMENT 13

int get_element_type(const char* element_name);
void get_element_type_name(char* ret_str, int type);
double get_multiplier_value(const char* multiplier_str);

union element_value_type_union
{
	double numeric_value;
	char string_value[256];
};

typedef union element_value_type_union element_value_type;

struct element_value_struct
{
	element_value_type value;
	int is_numeric;
};

typedef struct element_value_struct element_value;

struct element_struct
{
	int id;
	int type;
	char name[512];
	label_list* nodes;
	element_value value;
};

typedef struct element_struct element;

#endif
