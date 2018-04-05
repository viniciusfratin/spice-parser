#ifndef ELEMENT_LIST_H_
#define ELEMENT_LIST_H_

#include "generic_list.h"
#include "spice_format.h"

typedef generic_list element_list;

int element_list_initialize(element_list** list_ptr);
int element_list_insert(element_list** list_ptr, element value);
int element_list_contains_name(element_list* list, const char* element_name);
int element_list_clear(element_list** list_ptr);
void element_list_enumerate(element_list* list, void (*callback)(int, element, void*), void* additional_data);

#endif
