#ifndef LU_SOLVE_H_
#define LU_SOLVE_H_
void solve(double** lower_matrix, double** upper_matrix, double* independent_vector, double* solution_vector, int matrix_dim);
void apply_forward_elimination(double** lower_matrix, double* independent_vector, int matrix_dim);
void apply_backward_substition(double** upper_matrix, double* independent_vector, double* solution_vector, int matrix_dim);
void undo_permutation_vector(double* vector, double** permutation_matrix, double* result, int matrix_dim);

#endif
