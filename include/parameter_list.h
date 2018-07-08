#ifndef PARAMETER_LIST_H_
#define PARAMETER_LIST_H_

#include "generic_list.h"

struct parameter_struct
{
	char name[32];
	double value;
};

typedef struct parameter_struct parameter;

typedef generic_list parameter_list;

int parameter_list_initialize(parameter_list** list_ptr);
int parameter_list_insert(parameter_list** list_ptr, parameter value);
int parameter_list_count(parameter_list* list);
int parameter_list_contains_name(parameter_list* list, const char* parameter_name, parameter** ret_parameter);
int parameter_list_clear(parameter_list** list_ptr);
void parameter_list_enumerate(parameter_list* list, void (*callback)(int, parameter*, void*), void* additional_data);

#endif
