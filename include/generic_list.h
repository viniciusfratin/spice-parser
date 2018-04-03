#ifndef GENERIC_LIST_H_
#define GENERIC_LIST_H_

struct generic_list_struct
{
	void* value;
	struct generic_list_struct* prev;
	struct generic_list_struct* next;
};

typedef struct generic_list_struct generic_list;

int generic_list_initialize(generic_list** list_ptr);
int generic_list_insert(generic_list** list_ptr, void* value);
int generic_list_count(generic_list* list_ptr);
int generic_list_clear(generic_list** list_ptr);

#endif
