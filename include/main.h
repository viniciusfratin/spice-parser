#ifndef MAIN_H_
#define MAIN_H_

#include "label_list.h"
#include "element_list.h"

struct parser_data
{
	label_list** label_list;
	element_list** element_list;
	generic_list** command_list;
};

struct mna_data_struct
{
	label_list* nodes;
	element_list* elements;
};

typedef struct mna_data_struct mna_data;

#endif
