#include <stdio.h>
#include <stdlib.h>

#include "matrix_operations.h"
#include "lu_decomposition.h"

void decompose(double** coefficient_matrix, double* independent_vector, double** permutation_matrix, double** lower_matrix, double** upper_matrix, int matrix_dim)
{
	double** temp_matrix = alloc_matrix(matrix_dim);
	double* temp_vector = alloc_vector(matrix_dim);

	copy_matrix(temp_matrix, coefficient_matrix, matrix_dim);
	copy_vector(temp_vector, independent_vector, matrix_dim);

	int k;
	for(k = 0; k < matrix_dim; k++)
	{
		int j;
		get_max_in_row_range(temp_matrix, k, k, matrix_dim, &j);

		flip_rows(temp_matrix, temp_vector, permutation_matrix, k, j);

		int i;
		for(i = k + 1; i < matrix_dim; i++)
		{
			temp_matrix[i][k] = temp_matrix[i][k]/temp_matrix[k][k];

			for(j = k + 1; j < matrix_dim; j++)
			{
				temp_matrix[i][j] = temp_matrix[i][j] - temp_matrix[i][k] * temp_matrix[k][j];
			}

		}
	}

	int i, j;
	for(i = 0; i < matrix_dim; i++)
	{
		for(j = 0; j < matrix_dim; j++)
		{
			
			if(i <= j)
			{
				upper_matrix[i][j] = temp_matrix[i][j];
				if(i == j)
				{
					lower_matrix[i][j] = 1;
				}

				else
				{
					lower_matrix[i][j] = 0;
				}
			}

			else
			{
				upper_matrix[i][j] = 0;
				lower_matrix[i][j] = temp_matrix[i][j];
			}
		}
	}	

	free_matrix(temp_matrix, matrix_dim);
	free_vector(temp_vector);
}

void flip_rows(double **coefficient_matrix, double* independent_vector, double** permutation_matrix, int i, int j)
{
	double* coefficient_matrix_row_i = coefficient_matrix[i];
	double* coefficient_matrix_row_j = coefficient_matrix[j];
	coefficient_matrix[i] = coefficient_matrix_row_j;
	coefficient_matrix[j] = coefficient_matrix_row_i;

	double independent_vector_row_i = independent_vector[i];
	double independent_vector_row_j = independent_vector[j];
	independent_vector[i] = independent_vector_row_j;
	independent_vector[j] = independent_vector_row_i;

	double* permutation_matrix_row_i = permutation_matrix[i];
	double* permutation_matrix_row_j = permutation_matrix[j];
	permutation_matrix[i] = permutation_matrix_row_j;
	permutation_matrix[j] = permutation_matrix_row_i;
}


