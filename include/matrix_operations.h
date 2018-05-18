#ifndef MATRIX_OPERATIONS_H_
#define MATRIX_OPERATIONS_H_

double get_max_in_row_range(double** coefficient_matrix, int column, int initial_index, int final_index, int* ret_index);
double abs_val(double value);
double vector_norm(double* vector, int vector_dim);
void set_identity_matrix(double** matrix, int matrix_dim);
void set_null_matrix(double** matrix, int matrix_dim);
void set_null_vector(double* vector, int vector_dim);
double** alloc_matrix(int matrix_dim);
double* alloc_vector(int vector_dim);
void copy_matrix(double** dst, double** src, int matrix_dim);
void copy_vector(double* dst, double* src, int vector_dim);
void multiply_matrix_by_matrix(double** matrix_a, double** matrix_b, double** result, int matrix_dim);
void multiply_vector_by_matrix(double** matrix, double* vector, double* result, int vector_dim);
void sum_matrices(double** matrix_a, double** matrix_b, double** result, int matrix_dim);
void subtract_matrices(double** matrix_a, double** matrix_b, double** result, int matrix_dim);
void sum_vectors(double* vector_a, double* vector_b, double* result, int vector_dim);
void subtract_vectors(double* vector_a, double* vector_b, double* result, int vector_dim);
void print_matrix(double** matrix, int matrix_dim, char* matrix_name);
void print_vector(double* vector, int vector_dim, char* vector_name);
void free_matrix(double** matrix, int matrix_dim);
void free_vector(double* vector);

#endif
