#include <stdio.h>
#include <stdlib.h>

#include "matrix_operations.h"

double get_max_in_row_range(double** coefficient_matrix, int column, int initial_index, int final_index, int* ret_index)
{
	double max_value = abs_val(coefficient_matrix[initial_index][column]);
	if(ret_index != NULL)
	{
		*ret_index = initial_index;
	}

	int i;
	for(i = initial_index; i < final_index; i++)
	{
		double current_value = abs_val(coefficient_matrix[i][column]);
		if(current_value > max_value)
		{
			max_value = current_value;
			if(ret_index != NULL)
			{
				*ret_index = i;
			}
		}
	}

	return max_value;
}

double abs_val(double value)
{
	if(value < 0)
	{
		return -value;
	}

	return value;
}

double vector_norm(double* vector, int vector_dim)
{
	double max = vector[0];

	int i;
	for(i = 0; i < vector_dim; i++)
	{
		if(abs_val(vector[i]) > max)
		{
			max = abs_val(vector[i]);
		}
	}

	return max;
}

void set_identity_matrix(double** matrix, int matrix_dim)
{
	int i;
	for(int i = 0; i < matrix_dim; i++)
	{
		matrix[i][i] = 1;
	}
}

void set_null_matrix(double** matrix, int matrix_dim)
{
	int i, j;
	for(i = 0; i < matrix_dim; i++)
	{
		for(j = 0; j < matrix_dim; j++)
		{
			matrix[i][j] = 0;
		}
	}
}

void set_null_vector(double* vector, int vector_dim)
{
	int i;
	for(i = 0; i < vector_dim; i++)
	{
		vector[i] = 0;
	}
}

double** alloc_matrix(int matrix_dim)
{
	double **matrix = (double**) malloc(matrix_dim * sizeof(double*));
	if(matrix == NULL)
	{
		fprintf(stderr, "Alloc error.\n");
		exit(1);
	}	

	int i;
	for(i = 0; i < matrix_dim; i++)
	{
		matrix[i] = (double*) calloc(matrix_dim, sizeof(double));
		if(matrix[i] == NULL)
		{
			fprintf(stderr, "Alloc error.\n");
			exit(1);
		}
	}

	return matrix;

}

double* alloc_vector(int vector_dim)
{
	double* vector = (double*) calloc(vector_dim, sizeof(double));
	if(vector == NULL)
	{
		fprintf(stderr, "Alloc error.\n");
		exit(1);
	}

	return vector;
}

void copy_matrix(double** dst, double** src, int matrix_dim)
{
	int i, j;
	for(i = 0; i < matrix_dim; i++)
	{
		for(j = 0; j < matrix_dim; j++)
		{
			dst[i][j] = src[i][j];
		}
	}
}

void copy_vector(double* dst, double* src, int vector_dim)
{
	int i;
	for(i = 0; i < vector_dim; i++)
	{
		dst[i] = src[i];
	}
}

void multiply_matrix_by_matrix(double** matrix_a, double** matrix_b, double** result, int matrix_dim)
{
	int i, j, k;
	for(i = 0; i < matrix_dim; i++)
	{
		for(j = 0; j < matrix_dim; j++)
		{
			double sum = 0;
			for(k = 0; k < matrix_dim; k++)
			{
				sum += matrix_a[i][k] * matrix_b[k][j];
			}

			result[i][j] = sum;
		}
	}
}

void sum_matrices(double** matrix_a, double** matrix_b, double** result, int matrix_dim)
{
	int i, j;
	for(i = 0; i < matrix_dim; i++)
	{
		for(j = 0; j < matrix_dim; j++)
		{
			result[i][j] = matrix_a[i][j] + matrix_b[i][j];
		}
	}
}

void subtract_matrices(double** matrix_a, double** matrix_b, double** result, int matrix_dim)
{
	int i, j;
	for(i = 0; i < matrix_dim; i++)
	{
		for(j = 0; j < matrix_dim; j++)
		{
			result[i][j] = matrix_a[i][j] - matrix_b[i][j];
		}
	}
}

void sum_vectors(double* vector_a, double* vector_b, double* result, int vector_dim)
{
	int i;
	for(i = 0; i < vector_dim; i++)
	{
		result[i] = vector_a[i] + vector_b[i];
	}
}

void subtract_vectors(double* vector_a, double* vector_b, double* result, int vector_dim)
{
	int i;
	for(i = 0; i < vector_dim; i++)
	{
		result[i] = vector_a[i] - vector_b[i];
	}
}

void multiply_vector_by_matrix(double** matrix, double* vector, double* result, int vector_dim)
{
	int i, j;
	for(i = 0; i < vector_dim; i++)
	{
		double sum = 0;
		for(j = 0; j < vector_dim; j++)
		{
			sum += matrix[i][j] * vector[j];
		}

		result[i] = sum;
	}
}

void print_matrix(double** matrix, int matrix_dim, char* matrix_name)
{
	printf("\n");
	int i, j;
	for(i = 0; i < matrix_dim; i++)
	{
		for(j = 0; j < matrix_dim; j++)
		{
			printf("%s(%d,%d) = %le\n", matrix_name, i + 1, j + 1, matrix[i][j]);
		}		
	}	
}

void print_vector(double* vector, int vector_dim, char* vector_name)
{
	printf("\n");
	int i;
	for(i = 0; i < vector_dim; i++)
	{
		printf("%s(%d) = %le\n", vector_name, i + 1, vector[i]);
	}
}

void free_matrix(double** matrix, int matrix_dim)
{
	int i;
	for(i = 0; i < matrix_dim; i++)
	{
		free(matrix[i]);
	}

	free(matrix);
}

void free_vector(double* vector)
{
	free(vector);
}
