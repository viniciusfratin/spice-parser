#ifndef MNA_H
#define MNA_H

#include "element_list.h"
#include "label_list.h"

struct mna_data_struct
{
	label_list* nodes;
	element_list* elements;
};

typedef struct mna_data_struct mna_data;

int number_of_nodes;
int number_of_extra_currents;
int matrix_dim;
int *extra_currents_positions;
double **h_matrix;
double *b_vector;

#endif
