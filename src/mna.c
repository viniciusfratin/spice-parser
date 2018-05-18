#include <stdio.h>
#include <stdlib.h>

#include "mna.h"
#include "matrix_operations.h"
#include "build_matrices.h"
#include "lu_decomposition.h"
#include "iterative_refinement.h"

void start_mna(mna_data data)
{
	printf("\nStarting Modified Nodal Analysis...\n");

	int number_of_elements = element_list_count(data.elements);
	extra_currents_positions = (int*) malloc(number_of_elements * sizeof(int));
	if(extra_currents_positions == NULL)
	{
		fprintf(stderr, "Alloc error.\n");
		exit(1);
	}

	number_of_nodes = label_list_count(data.nodes);
	number_of_extra_currents = 0;
	printf("\nClassifying elements into groups...\n");
	element_list_enumerate(data.elements, &classify_element_groups, (void*)&data.elements);

	printf("\nClassification result:\n");
	element_list_enumerate(data.elements, &list_element_groups, NULL);

	
	printf("\nAllocating matrices...\n");
	matrix_dim = number_of_nodes + number_of_extra_currents - 1; // Ground doesn't enter the equations.
	
	h_matrix = alloc_matrix(matrix_dim);
	upper_matrix = alloc_matrix(matrix_dim);
	lower_matrix = alloc_matrix(matrix_dim);
	permutation_matrix = alloc_matrix(matrix_dim);

	b_vector = alloc_vector(matrix_dim);	
	x_vector = alloc_vector(matrix_dim);
	
	printf("\nAllocation successfull.\n");	

	set_identity_matrix(permutation_matrix, matrix_dim);

	printf("\nGenerating element stamps...\n");
	element_list_enumerate(data.elements, &generate_element_stamps, NULL);

	printf("\nLinear system Hx = B creation successfull.\n");
	print_matrices();

	printf("\nStarting LU Decomposition...\n");
	decompose(h_matrix, b_vector, permutation_matrix, lower_matrix, upper_matrix, matrix_dim);

	printf("\nLU Decomposition successfull.\n");
	
	printf("\nSolving system of equations...\n");
	refine(h_matrix, lower_matrix, upper_matrix, permutation_matrix, b_vector, x_vector, 0.01, 0.1, matrix_dim);	
	
	printf("\nEquation solving successfull.\n");
	print_solution();

	free_matrices();
}

void print_matrices()
{
	print_matrix(h_matrix, matrix_dim, "H");
	print_vector(b_vector, matrix_dim, "B");
}

void print_solution()
{
	print_vector(x_vector, matrix_dim, "x");
}

void free_matrices()
{
	free_matrix(h_matrix, matrix_dim);
	free_matrix(lower_matrix, matrix_dim);
	free_matrix(upper_matrix, matrix_dim);
	free_matrix(permutation_matrix, matrix_dim);
	
	free_vector(b_vector);
	free_vector(x_vector);

	free(extra_currents_positions);
}
