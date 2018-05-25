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
void** matrix_pointers;
double **h_matrix;
double **lower_matrix;
double **upper_matrix;
double *b_vector;
double *x_vector;

double **permutation_matrix;

void start_mna(mna_data data);
void print_matrices();
void print_solution(mna_data data);
void free_matrices();

#endif
