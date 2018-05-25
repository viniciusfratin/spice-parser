#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "label_list.h"
#include "string_operations.h"

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
		if(label_list_contains_name(*list_ptr, label_name, NULL) != -1)
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
	
	strcpy(alloc_value->name, label_name);
	// Alloc error.
	if(alloc_value->name == NULL)
	{
		free(alloc_value);

		fprintf(stderr, "Insufficient memory.\n");
		exit(1);
		return 1;
	}

	alloc_value->id = element_count;

	int result = generic_list_insert(list_ptr, (void*) alloc_value);
	// Insert error.
	if(result != 0)
	{
		free(alloc_value);

		fprintf(stderr, "Insertion failed.\n");
		exit(1);
		return 1;
	}

	return 0;
}

int label_list_count(label_list* list)
{
	return generic_list_count(list);
}

int label_list_contains_name(label_list* list, const char* label_name, label** ret_label)
{
	int found_index = -1;

	char lowercase_name[512];
	strcpy(lowercase_name, label_name);
	convert_to_lowercase(lowercase_name);

	int current_index;
	label_list* current_element = list;
	for(current_index = 0; current_element != NULL; current_index++, current_element = current_element->next)
	{		
		label* l = (label*) current_element->value;
		char lowercase_current_name[512];
		strcpy(lowercase_current_name, l->name);
		convert_to_lowercase(lowercase_current_name);
		if(strcmp(lowercase_current_name, lowercase_name) == 0)
		{
			if(ret_label != NULL)
			{
				*ret_label = l;	
			}

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
		free(l);

		current_element = current_element->next;
	}

	int clear_result = generic_list_clear(list_ptr);
	return clear_result;
}

void label_list_enumerate(label_list* list, void (*callback)(int, label*, void*), void* additional_data)
{
	int i;
	label_list* current_element = list;
	for(i = 0; current_element != NULL; current_element = current_element->next, i++)
	{
		label* l = (label*) current_element->value;
		(*callback)(i, l, additional_data);
	}
}
