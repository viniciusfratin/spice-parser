#include "lu_solve.h"
#include "matrix_operations.h"

void solve(double** lower_matrix, double** upper_matrix, double* independent_vector, double* solution_vector, int matrix_dim)
{
	double** temp_lower_matrix = alloc_matrix(matrix_dim);
	double** temp_upper_matrix = alloc_matrix(matrix_dim);
	double* temp_independent_vector = alloc_vector(matrix_dim);

	copy_matrix(temp_lower_matrix, lower_matrix, matrix_dim);
	copy_matrix(temp_upper_matrix, upper_matrix, matrix_dim);
	copy_vector(temp_independent_vector, independent_vector, matrix_dim);

	apply_forward_elimination(temp_lower_matrix, temp_independent_vector, matrix_dim);
	apply_backward_substition(temp_upper_matrix, temp_independent_vector, solution_vector, matrix_dim);

	free_matrix(temp_lower_matrix, matrix_dim);
	free_matrix(temp_upper_matrix, matrix_dim);
	free_vector(temp_independent_vector);
}

void apply_forward_elimination(double** lower_matrix, double* independent_vector, int matrix_dim)
{
	int k;
	for(k = 0; k < matrix_dim - 1; k++)
	{
		int i;
		for(i = k + 1; i < matrix_dim; i++)
		{
			double m = lower_matrix[i][k]/lower_matrix[k][k];
			lower_matrix[i][k] = 0;

			int j;
			for(j = k + 1; j < matrix_dim; j++)
			{
				lower_matrix[i][j] = lower_matrix[i][j] - m * lower_matrix[k][j];
			}

			independent_vector[i] = independent_vector[i] - m * independent_vector[k];
		}
	}
}

void apply_backward_substition(double** upper_matrix, double* independent_vector, double* solution_vector, int matrix_dim)
{
	int k;
	for(k = matrix_dim - 1; k >= 0; k--)
	{
		solution_vector[k] = independent_vector[k];

		int j;
		for(j = k + 1; j < matrix_dim; j++)
		{
			solution_vector[k] = solution_vector[k] - upper_matrix[k][j] * solution_vector[j];
		}

		solution_vector[k] = solution_vector[k]/upper_matrix[k][k];
	}
}

void undo_permutation_vector(double* vector, double** permutation_matrix, double* result, int matrix_dim)
{
	int i;
	for(i = 0; i < matrix_dim; i++)
	{
		double sum = 0;
	
		int j;
		for(j = 0; j < matrix_dim; j++)
		{
			sum += permutation_matrix[j][i] * vector[j];
		}

		result[i] = sum;
	}
}

