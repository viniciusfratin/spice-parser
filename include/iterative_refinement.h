#ifndef ITERATIVE_REFINEMENT_H_
#define ITERATIVE_REFINEMENT_H_

void refine(double** coefficient_matrix, double** lower_matrix, double** upper_matrix, double** permutation_matrix, double* independent_vector, double* solution_vector, double reltol, double abstol, int matrix_dim);
void iterate(double** coefficient_Matrix, double** lower_matrix, double** upper_matrix, double** permutation_matrix, double* independent_vector, double* solution_vector, double* z_vector, double* temp_vector, double* residue, int matrix_dim);

#endif
