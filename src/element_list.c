#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element_list.h"
#include "string_operations.h"

int element_list_initialize(element_list** list_ptr)
{
	int result = generic_list_initialize(list_ptr);
	return result;
}


int element_list_insert(element_list** list_ptr, element value)
{
	int element_count = 0;
	// List already initialized.
	if(list_ptr != NULL)
	{		
		// Already contains element.	
		if(element_list_contains_name(*list_ptr, value.name, NULL) != -1)
		{
			fprintf(stderr, "Element %s already exists.\n", value.name);
			exit(1);
			return 1;
		}

		element_count = generic_list_count(*list_ptr);
	}
	element* alloc_value = (element*) malloc(sizeof(element));
	// Alloc error.
	if(alloc_value == NULL)
	{
		fprintf(stderr, "Insufficent memory.\n");
		exit(1);
		return 1;
	}
	
	alloc_value->id = element_count;
	alloc_value->type = value.type;
	strcpy(alloc_value->name, value.name);
	alloc_value->nodes = value.nodes;
	alloc_value->ref_elements = value.ref_elements;
	alloc_value->value = value.value;
	
	int result = generic_list_insert(list_ptr, (void*) alloc_value);
	// Insert error.
	if(result != 0)
	{
		free(alloc_value);

		fprintf(stderr, "Element insertion failed.\n");
		exit(1);
		return 1;
	}

	return 0;
}

int element_list_count(element_list* list)
{
	return generic_list_count(list);
}

int element_list_contains_name(element_list* list, const char* element_name, element** ret_element)
{
	int found_index = -1;

	char lowercase_name[512];
	strcpy(lowercase_name, element_name);
	convert_to_lowercase(lowercase_name);

	int current_index;
	element_list* current_element = list;
	for(current_index = 0; current_element != NULL; current_index++, current_element = current_element->next)
	{		
		element* l = (element*) current_element->value;
		char lowercase_current_name[512];
		strcpy(lowercase_current_name, l->name);
	 	convert_to_lowercase(lowercase_current_name);
		if(strcmp(lowercase_current_name, lowercase_name) == 0)
		{
			found_index = current_index;
			
			if(ret_element != NULL)
			{
				*ret_element = l;
			}

			break;
		}
	}
	
	return found_index;
}

int element_list_clear(element_list** list_ptr)
{
	// Invalid list.
	if(list_ptr == NULL)
	{
		return 1;
	}

	element_list* list = *list_ptr;
	element_list* current_element = list;
	while(current_element != NULL)
	{
		element* l = (element*) current_element->value;
		free(l);

		generic_list_clear(&l->nodes);

		current_element = current_element->next;
	}

	int clear_result = generic_list_clear(list_ptr);
	return clear_result;
}

void element_list_enumerate(element_list* list, void (*callback)(int, element*, void*), void* additional_data)
{
	int i;
	element_list* current_element = list;
	for(i = 0; current_element != NULL; current_element = current_element->next, i++)
	{
		element* l = (element*) current_element->value;
		(*callback)(i, l, additional_data);
	}
}
