#include <stdio.h>
#include <stdlib.h>

#include "mna.h"
#include "build_matrices.h"

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
	
	h_matrix = (double**) malloc(matrix_dim * sizeof(double*));
	if(h_matrix == NULL)
	{
		fprintf(stderr, "Alloc error.\n");
		exit(1);
	}	

	int i;
	for(i = 0; i < matrix_dim; i++)
	{
		h_matrix[i] = (double*) calloc(matrix_dim, sizeof(double));
		if(h_matrix[i] == NULL)
		{
			fprintf(stderr, "Alloc error.\n");
			exit(1);
		}
	}

	b_vector = (double*) calloc(matrix_dim, sizeof(double));
	if(b_vector == NULL)
	{
		fprintf(stderr, "Alloc error.\n");
		exit(1);
	}

	printf("\nAllocation successfull.\n");	

	printf("\nGenerating element stamps...\n");
	element_list_enumerate(data.elements, &generate_element_stamps, NULL);

	printf("\nLinear system Hx = B creation successfull.\n");
	print_matrices();
}


