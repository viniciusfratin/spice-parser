#ifndef SPICE_FORMAT_H_
#define SPICE_FORMAT_H_

#include "label_list.h"
#include "parameter_list.h"

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
#define TYPE_INVALID_ELEMENT -1

#define COMMAND_OP 1
#define COMMAND_INVALID -1

#define MODEL_DIODEM1 1
#define MODEL_NMOS 2
#define MODEL_PMOS 3
#define MODEL_INVALID -1

int get_element_type(const char* element_name);
void get_element_type_name(char* ret_str, int type);
double get_multiplier_value(const char* multiplier_str);
int get_command_type(const char* command_name);

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

struct number_desc_struct
{
	char string[256];
	double number;
};

typedef struct number_desc_struct number_desc;

struct element_struct
{
	int id;
	int group;
	int type;
	char name[512];
	int is_set;
	generic_list* nodes; // generic_list of label*
	generic_list* ref_elements; // generic_list of element*
	element_value value;
	int model;
	parameter_list* param_list;
	generic_list* companion_elements;
};

typedef struct element_struct element;

struct command_struct
{
	int type;
	generic_list* parameters;
};

typedef struct command_struct command;

struct special_cc_struct
{
	int is_cc;
	char string[512];
};

typedef struct special_cc_struct special_cc;


#endif
