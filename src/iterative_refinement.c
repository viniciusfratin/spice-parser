#include <stdio.h>

#include "iterative_refinement.h"
#include "matrix_operations.h"
#include "lu_solve.h"

void refine(double** coefficient_matrix, double** lower_matrix, double** upper_matrix, double** permutation_matrix, double* independent_vector, double* solution_vector, double reltol, double abstol, int matrix_dim)
{
	double* z_vector = alloc_vector(matrix_dim);
	double* temp_vector = alloc_vector(matrix_dim);
	double* residue = alloc_vector(matrix_dim);

	set_null_vector(solution_vector, matrix_dim);

	double z_norm;
	double x_norm;
	int iteration_counter = 1;
	do {
		printf("-> Refinement iteration 1...\n");
		iterate(coefficient_matrix, lower_matrix, upper_matrix, permutation_matrix, independent_vector, solution_vector, z_vector, temp_vector, residue, matrix_dim);

		z_norm = vector_norm(z_vector, matrix_dim);
		x_norm = vector_norm(solution_vector, matrix_dim);

		iteration_counter++;
	} while(z_norm <= reltol * x_norm + abstol);

	free_vector(z_vector);
	free_vector(temp_vector);
	free_vector(residue);
}

void iterate(double** coefficient_matrix, double** lower_matrix, double** upper_matrix, double** permutation_matrix, double* independent_vector, double* solution_vector, double* z_vector, double* temp_vector, double* residue, int matrix_dim)
{
	multiply_vector_by_matrix(coefficient_matrix, solution_vector, temp_vector, matrix_dim);
	subtract_vectors(independent_vector, temp_vector, residue, matrix_dim);
	
	multiply_vector_by_matrix(permutation_matrix, residue, temp_vector, matrix_dim);

	// Non-destructive.	
	solve(lower_matrix, upper_matrix, temp_vector, z_vector, matrix_dim);

	sum_vectors(solution_vector, z_vector, solution_vector, matrix_dim);
}
