#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameter_list.h"
#include "string_operations.h"

int parameter_list_initialize(parameter_list** list_ptr)
{
	int result = generic_list_initialize(list_ptr);
	return result;
}

int parameter_list_insert(parameter_list** list_ptr, parameter value)
{
	parameter* alloc_value = (parameter*) malloc(sizeof(parameter));
	// Alloc error.
	if(alloc_value == NULL)
	{
		return 1;
	}

	strcpy(alloc_value->name, value.name);
	alloc_value->value = value.value;

	int result = generic_list_insert(list_ptr, (void*) alloc_value);
	if(result != 0)
	{
		free(alloc_value);
	
		fprintf(stderr, "Insertion failed.\n");
		exit(1);
		return 1;
	}

	return 0;
}

int parameter_list_count(parameter_list* list)
{
	return parameter_list_count(list);
}

int parameter_list_contains_name(parameter_list* list, const char* parameter_name, parameter** ret_parameter)
{
	int found_index = -1;

	char lowercase_name[512];
	strcpy(lowercase_name, parameter_name);
	convert_to_lowercase(lowercase_name);

	int current_index;
	parameter_list* current_element = list;
	for(current_index = 0; current_element != NULL; current_index++, current_element = current_element->next)
	{		
		parameter* p = (parameter*) current_element->value;
		char lowercase_current_name[512];
		strcpy(lowercase_current_name, p->name);
		convert_to_lowercase(lowercase_current_name);
		if(strcmp(lowercase_current_name, lowercase_name) == 0)
		{
			if(ret_parameter != NULL)
			{
				*ret_parameter = p;	
			}

			found_index = current_index;
			break;
		}
	}
	
	return found_index;
}

int parameter_list_clear(parameter_list** list_ptr)
{
	// Invalid list.
	if(list_ptr == NULL)
	{
		return 1;
	}

	parameter_list* list = *list_ptr;
	parameter_list* current_element = list;
	while(current_element != NULL)
	{
		parameter* l = (parameter*) current_element->value;
		free(l);

		current_element = current_element->next;
	}

	int clear_result = generic_list_clear(list_ptr);
	return clear_result;
}

void parameter_list_enumerate(parameter_list* list, void (*callback)(int, parameter*, void*), void* additional_data)
{
	int i;
	parameter_list* current_element = list;
	for(i = 0; current_element != NULL; current_element = current_element->next, i++)
	{
		parameter* p = (parameter*) current_element->value;
		(*callback)(i, p, additional_data);
	}
}
