#include <stdio.h>
#include <stdlib.h>

#include "mna.h"
#include "matrix_operations.h"
#include "build_matrices.h"
#include "lu_decomposition.h"
#include "iterative_refinement.h"

#define ABSTOL 1e-6
#define RELTOL 1e-3

//#define VERBOSE

void start_mna(mna_data data)
{
	printf("\nStarting Modified Nodal Analysis...\n");

	printf("\nVerifying non-linear element models...\n");
	element_list_enumerate(data.elements, &check_non_linear_models, NULL);

	printf("\nGenerating non-linear elements companions...\n");
	element_list_enumerate(data.elements, &generate_non_linear_companions, (void*)&data.elements);

	int number_of_elements = element_list_count(data.elements);
	extra_currents_positions = (int*) malloc(number_of_elements * sizeof(int));
	if(extra_currents_positions == NULL)
	{
		fprintf(stderr, "Alloc error.\n");
		exit(1);
	}
	
	number_of_nodes = label_list_count(data.nodes);

	matrix_pointers = (void*) malloc((number_of_nodes + number_of_elements) * sizeof(void*));
	if(matrix_pointers == NULL)
	{
		fprintf(stderr, "Alloc error.\n");
		exit(1);
	}

	label_list_enumerate(data.nodes, &set_nodes, NULL);	

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
	double* previous_x_vector = alloc_vector(matrix_dim);
	double* correction_vector = alloc_vector(matrix_dim);	
	double tolerance = 0;
	double correction = 1;

	printf("\nAllocation successfull.\n");	

	printf("\nBegin solving...");

	// Solving begin.
	int i = 0;
	while(correction > tolerance) 
	{
		printf("\nMain iteration %d...\n", i + 1);

		set_identity_matrix(permutation_matrix, matrix_dim);
		set_null_matrix(h_matrix, matrix_dim);
		set_null_matrix(lower_matrix, matrix_dim);
		set_null_matrix(upper_matrix, matrix_dim);
		set_null_vector(b_vector, matrix_dim);
	
#ifdef VERBOSE
		printf("\nGenerating non-linear initial stamps...");
#endif
		element_list_enumerate(data.elements, &set_companion_values, (void*)previous_x_vector);

#ifdef VERBOSE
		printf("\nGenerating linear element stamps...\n");
#endif
		element_list_enumerate(data.elements, &generate_element_stamps, NULL);
	
#ifdef VERBOSE
		printf("\nLinear system Hx = B creation successfull.\n");
		print_matrices();
	
		printf("\nStarting LU Decomposition...\n");
#endif
		decompose(h_matrix, b_vector, permutation_matrix, lower_matrix, upper_matrix, matrix_dim);
	
#ifdef VERBOSE
		printf("\nLU Decomposition successfull.\n");
		
		printf("\nSolving system of equations...\n");
#endif
		if(i == 0)
		{
			refine(h_matrix, lower_matrix, upper_matrix, permutation_matrix, b_vector, previous_x_vector, RELTOL, ABSTOL, matrix_dim);
			tolerance = RELTOL * vector_norm(previous_x_vector, matrix_dim);
			correction = tolerance + 1;
		}
	
		else
		{
			refine(h_matrix, lower_matrix, upper_matrix, permutation_matrix, b_vector, x_vector, RELTOL, ABSTOL, matrix_dim);
			subtract_vectors(x_vector, previous_x_vector, correction_vector, matrix_dim);
			correction = vector_norm(correction_vector, matrix_dim);
	
			copy_vector(previous_x_vector, x_vector, matrix_dim);
		}

		i++;
	}

	printf("\nEquation solving successfull.\n");
	print_solution(data);


	free(previous_x_vector);
	free(correction_vector);
	free_matrices();
}

void print_matrices()
{
	print_matrix(h_matrix, matrix_dim, "H");
	print_vector(b_vector, matrix_dim, "B");
}

void print_solution(mna_data data)
{
	printf("\nSolution:\n");

	int i;
	for(i = 0; i < number_of_nodes - 1; i++)
	{
		printf("V(%s) = %le\n", ((label*)matrix_pointers[i])->name, x_vector[i]);
	}

	printf("\n");
	for(i = number_of_nodes - 1; i < matrix_dim; i++)
	{
		printf("I(%s) = %le\n", ((element*)matrix_pointers[i])->name, x_vector[i]);
	}
}

void free_matrices()
{
	free_matrix(h_matrix, matrix_dim);
	free_matrix(lower_matrix, matrix_dim);
	free_matrix(upper_matrix, matrix_dim);
	free_matrix(permutation_matrix, matrix_dim);
	
	free_vector(b_vector);
	free_vector(x_vector);

	free(matrix_pointers);
	free(extra_currents_positions);
}
