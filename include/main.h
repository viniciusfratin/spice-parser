#ifndef MAIN_H_
#define MAIN_H_

#include "label_list.h"

struct lexer_data
{
	label_list* global_label_list;
	label_list* current_label_list;
};

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

#endif
