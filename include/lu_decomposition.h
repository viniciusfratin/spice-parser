#ifndef LU_DECOMPOSITION_H_
#define LU_DECOMPOSITION_H_

void decompose(double** coefficient_matrix, double* independent_vector, double** permutation_matrix, double** lower_matrix, double** upper_matrix, int matrix_dim);
void flip_rows(double **coefficient_matrix, double* independent_vector, double** permutation_matrix, int i, int j);

#endif
