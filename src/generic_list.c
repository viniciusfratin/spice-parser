#include <stdio.h>
#include <stdlib.h>
#include "generic_list.h"

int generic_list_initialize(generic_list** list_ptr)
{
	// Invalid list.
	if(list_ptr == NULL)
	{
		return 1;
	}

	*list_ptr = NULL;

	return 0;
}

int generic_list_insert(generic_list** list_ptr, void* value)
{
	// Invalid list.
	if(list_ptr == NULL)
	{
		return 1;
	}

	generic_list* list = *list_ptr;
	// Empty list.
	if(list == NULL)
	{
		list = (generic_list*) malloc(sizeof(generic_list));
		// Alloc error.
		if(list == NULL)
		{
			fprintf(stderr, "Insufficient memory.\n");
			exit(1);
			return 1;
		}

		list->prev = NULL;
		list->next = NULL;
		list->value = value;

		*list_ptr = list;
	}

	else
	{
		generic_list* new_element = (generic_list*) malloc(sizeof(generic_list));
		// Alloc error.
		if(new_element == NULL)
		{
			fprintf(stderr, "Insufficient memory.\n");
			exit(1);
			return 1;
		}

		generic_list* last_element = list;
		while(last_element->next != NULL)
		{
			last_element = last_element->next;
		}

		last_element->next = new_element;
		new_element->prev = last_element;
		new_element->value = value;		
		new_element->next = NULL;
	}

	return 0;
}

int generic_list_count(generic_list* list_ptr)
{
	int count = 0;

	generic_list* current_element = list_ptr;
	while(current_element != NULL)
	{
		count++;
		current_element = current_element->next;
	}

	return count;
}

int generic_list_clear(generic_list** list_ptr)
{
	// Invalid list.
	if(list_ptr == NULL)
	{
		return 1;
	}

	generic_list* current_element = *list_ptr;
	while(current_element != NULL)
	{
		generic_list* next = current_element->next;
		current_element->prev = NULL;
		current_element->next = NULL;
		current_element->value = NULL;

		free(current_element);
		
		current_element = next;
	}

	*list_ptr = NULL;

	return 0;
}

void generic_list_enumerate(generic_list* list, void (*callback)(int, void*, void*), void* additional_data)
{
	int i;
	generic_list* current_element = list;
	for(i = 0; current_element != NULL; current_element = current_element->next, i++)
	{
		void* l = current_element->value;
		(*callback)(i, l, additional_data);
	}

}

