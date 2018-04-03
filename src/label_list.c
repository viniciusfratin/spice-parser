#include <stdlib.h>
#include <string.h>
#include "label_list.h"

int label_list_initialize(label_list** list_ptr)
{
	int result = generic_list_initialize(list_ptr);
	return result;
}


int label_list_insert(label_list** list_ptr, const char* label_name)
{
	int element_count = 0;
	// List already initialized.
	if(list_ptr != NULL)
	{		
		// Already contains label.	
		if(label_list_contains_name(*list_ptr, label_name) != -1)
		{
			return 0;
		}

		element_count = generic_list_count(*list_ptr);
	}

	label* alloc_value = (label*) malloc(sizeof(label));
	// Alloc error.
	if(alloc_value == NULL)
	{
		return 1;
	}
	
	alloc_value->name = strdup(label_name);
	// Alloc error.
	if(alloc_value->name == NULL)
	{
		free(alloc_value);
		return 1;
	}

	alloc_value->id = element_count;

	int result = generic_list_insert(list_ptr, (void*) alloc_value);
	// Insert error.
	if(result != 0)
	{
		free(alloc_value->name);
		free(alloc_value);
		return 1;
	}

	return 0;
}

int label_list_contains_name(label_list* list, const char* label_name)
{
	int found_index = -1;

	int current_index;
	label_list* current_element = list;
	for(current_index = 0; current_element != NULL; current_index++, current_element = current_element->next)
	{
		label* l = (label*) current_element->value;
		if(strcmp(l->name, label_name) == 0)
		{
			found_index = current_index;
			break;
		}
	}
	
	return found_index;
}

int label_list_clear(label_list** list_ptr)
{
	// Invalid list.
	if(list_ptr == NULL)
	{
		return 1;
	}

	label_list* list = *list_ptr;
	label_list* current_element = list;
	while(current_element != NULL)
	{
		label* l = (label*) current_element->value;
		free(l->name);
		free(l);

		current_element = current_element->next;
	}

	int clear_result = generic_list_clear(list_ptr);
	return clear_result;
}
