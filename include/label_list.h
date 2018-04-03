#ifndef LABEL_LIST_H_
#define LABEL_LIST_H_

#include "generic_list.h"

typedef generic_list label_list;

struct label_struct
{
	int id;
	char* name;
};

typedef struct label_struct label;

int label_list_initialize(label_list** list_ptr);
int label_list_insert(label_list** list_ptr, const char* label_name);
int label_list_contains_name(label_list* list, const char* label_name);
int label_list_clear(label_list** list_ptr);

#endif
