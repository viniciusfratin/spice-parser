#include <stdio.h>
#include <stdlib.h>

#include "generic_list.h"
#include "mna.h"
#include "build_matrices.h"

void classify_element_groups(int position, element* element_ptr, void* additional_data)
{
	element_list* elements = *((element_list**)additional_data);
	// Default.
	element_ptr->group = 1;

	switch(element_ptr->type)
	{
		case TYPE_CCV_SOURCE:
			element_ptr->group = 2;
			number_of_extra_currents++;
			extra_currents_positions[position] = number_of_extra_currents + number_of_nodes - 1;

		case TYPE_CCC_SOURCE:
			;
			element* ref_elem = (element*)element_ptr->ref_elements->value;
			ref_elem->group = 2;
			number_of_extra_currents++;
			extra_currents_positions[ref_elem->id] = number_of_extra_currents + number_of_nodes - 1;
			if(element_ptr->type == TYPE_CCC_SOURCE)
			{
				extra_currents_positions[position] = -1;
			}
			printf("-> Changed element %s to group 2 due to element %s\n", ref_elem->name, element_ptr->name);
			break;

		case TYPE_V_SOURCE:
		case TYPE_VCV_SOURCE:
			element_ptr->group = 2;
			number_of_extra_currents++;
			extra_currents_positions[position] = number_of_extra_currents + number_of_nodes - 1;
			break;

		default:
			extra_currents_positions[position] = -1;
			break;
	}
}

void generate_element_stamps(int position, element* element_ptr, void* additional_data)
{
	switch(element_ptr->type)
	{
		case TYPE_RESISTOR:
			generate_resistor_stamp(element_ptr);
			break;

		case TYPE_C_SOURCE:
			generate_current_source_stamp(element_ptr);
			break;

		case TYPE_V_SOURCE:
			generate_voltage_source_stamp(element_ptr);
			break;

		case TYPE_VCV_SOURCE:
			generate_vcv_source_stamp(element_ptr);
			break;

		case TYPE_CCV_SOURCE:
			generate_ccv_source_stamp(element_ptr);
			break;

		case TYPE_CCC_SOURCE:
			generate_ccc_source_stamp(element_ptr);
			break;

		case TYPE_VCC_SOURCE:
			generate_vcc_source_stamp(element_ptr);
			break;

		default:
			fprintf(stderr, "Unsupported element.\n");
			exit(1);
	}
}

void generate_resistor_stamp(element* element_ptr)
{
	double value = element_ptr->value.value.numeric_value;
	int node_plus_id = ((label*)element_ptr->nodes->value)->id;
	int node_minus_id = ((label*)element_ptr->nodes->next->value)->id;
	
	switch(element_ptr->group)
	{
		case 1:
			if(node_plus_id != 0)
			{
				h_matrix[node_plus_id - 1][node_plus_id - 1] += 1/value;
				
				if(node_minus_id != 0)
				{
					h_matrix[node_plus_id - 1][node_minus_id - 1] += -1/value;
				}
			}

			if(node_minus_id != 0)
			{
				h_matrix[node_minus_id - 1][node_minus_id - 1] += 1/value;
				
				if(node_plus_id != 0)
				{
					h_matrix[node_minus_id - 1][node_plus_id - 1] += -1/value;
				}
			}

			break;

		case 2:
			;
			int current_id = extra_currents_positions[element_ptr->id];
			if(node_plus_id != 0)
			{
				h_matrix[node_plus_id - 1][current_id - 1] += 1;
				h_matrix[current_id - 1][node_plus_id - 1] += 1;
			}

			if(node_minus_id != 0)
			{
				h_matrix[node_minus_id - 1][current_id - 1] += -1;
				h_matrix[current_id - 1][node_minus_id - 1] += -1;
			}

			h_matrix[current_id - 1][current_id - 1] += -value;
			break;
	}
}

void generate_current_source_stamp(element* element_ptr)
{
	double value = element_ptr->value.value.numeric_value;
	int node_plus_id = ((label*)element_ptr->nodes->value)->id;
	int node_minus_id = ((label*)element_ptr->nodes->next->value)->id;

	switch(element_ptr->group)
	{
		case 1:
			if(node_plus_id != 0)
			{
				b_vector[node_plus_id - 1] += -value;
			}

			if(node_minus_id != 0)
			{
				b_vector[node_minus_id - 1] += value;
			}

			break;
		
		case 2:
			;
			int current_id = extra_currents_positions[element_ptr->id];
			if(node_plus_id != 0)
			{
				h_matrix[node_plus_id - 1][current_id - 1] += 1;
			}

			if(node_minus_id != 0)
			{
				h_matrix[node_minus_id - 1][current_id - 1] += -1;
			}

			h_matrix[current_id - 1][current_id - 1] += 1;
			b_vector[current_id - 1] += value;

			break;
	}
}

void generate_voltage_source_stamp(element* element_ptr)
{
	double value = element_ptr->value.value.numeric_value;
	int node_plus_id = ((label*)element_ptr->nodes->value)->id;
	int node_minus_id = ((label*)element_ptr->nodes->next->value)->id;
	int current_id = extra_currents_positions[element_ptr->id];

	if(node_plus_id != 0)
	{
		h_matrix[node_plus_id - 1][current_id - 1] += 1;
		h_matrix[current_id - 1][node_plus_id - 1] += 1;
	}

	if(node_minus_id != 0)
	{
		h_matrix[node_minus_id - 1][current_id - 1] += -1;
		h_matrix[current_id - 1][node_minus_id - 1] += -1;
	}

	b_vector[current_id - 1] += value;
}

void generate_vcv_source_stamp(element* element_ptr)
{
	double value = element_ptr->value.value.numeric_value;
	int node_plus_id = ((label*)element_ptr->nodes->value)->id;
	int node_minus_id = ((label*)element_ptr->nodes->next->value)->id;
	int x_node_plus_id = ((label*)element_ptr->nodes->next->next->value)->id;
	int x_node_minus_id = ((label*)element_ptr->nodes->next->next->next->value)->id;	
	int current_id = extra_currents_positions[element_ptr->id];

	if(node_plus_id != 0)
	{
		h_matrix[node_plus_id - 1][current_id - 1] += 1;
		h_matrix[current_id - 1][node_plus_id - 1] += 1;
	}

	if(node_minus_id != 0)
	{
		h_matrix[node_minus_id - 1][current_id - 1] += -1;
		h_matrix[current_id - 1][node_minus_id - 1] += -1;
	}

	if(x_node_plus_id != 0)
	{
		h_matrix[current_id - 1][x_node_plus_id - 1] += -value;
	}

	if(x_node_minus_id != 0)
	{
		h_matrix[current_id - 1][x_node_minus_id - 1] += value;
	}
}

void generate_ccv_source_stamp(element* element_ptr)
{
	double value = element_ptr->value.value.numeric_value;
	int node_plus_id = ((label*)element_ptr->nodes->value)->id;
	int node_minus_id = ((label*)element_ptr->nodes->next->value)->id;
	int current_id = extra_currents_positions[element_ptr->id];
	int ref_element_id = ((element*)element_ptr->ref_elements->value)->id;
	int x_current_id = extra_currents_positions[ref_element_id];
	
	if(node_plus_id != 0)
	{
		h_matrix[node_plus_id - 1][current_id - 1] += 1;
		h_matrix[current_id - 1][node_plus_id - 1] += 1;
	}

	if(node_minus_id != 0)
	{
		h_matrix[node_minus_id - 1][current_id - 1] += -1;
		h_matrix[current_id - 1][node_minus_id - 1] += -1;
	}

	h_matrix[current_id - 1][x_current_id - 1] += -value;
}

void generate_ccc_source_stamp(element* element_ptr)
{
	double value = element_ptr->value.value.numeric_value;
	int node_plus_id = ((label*)element_ptr->nodes->value)->id;
	int node_minus_id = ((label*)element_ptr->nodes->next->value)->id;
	int ref_element_id = ((element*)element_ptr->ref_elements->value)->id;
	int x_current_id = extra_currents_positions[ref_element_id];
	switch(element_ptr->group)
	{
		case 1:
			if(node_plus_id != 0)
			{
				h_matrix[node_plus_id - 1][x_current_id - 1] += value;
			}

			if(node_minus_id != 0)
			{
				h_matrix[node_minus_id - 1][x_current_id - 1] += -value;
			}

			break;
		
		case 2:
			;
			int current_id = extra_currents_positions[element_ptr->id];
			if(node_plus_id != 0)
			{
				h_matrix[node_plus_id - 1][current_id - 1] += 1;
			}

			if(node_minus_id != 0)
			{
				h_matrix[node_minus_id - 1][current_id - 1] += -1;
			}

			h_matrix[current_id - 1][current_id - 1] += 1;
			h_matrix[current_id - 1][x_current_id - 1] += -value;

			break;
	}
}

void generate_vcc_source_stamp(element* element_ptr)
{
	double value = element_ptr->value.value.numeric_value;
	int node_plus_id = ((label*)element_ptr->nodes->value)->id;
	int node_minus_id = ((label*)element_ptr->nodes->next->value)->id;
	int x_node_plus_id = ((label*)element_ptr->nodes->next->next->value)->id;
	int x_node_minus_id = ((label*)element_ptr->nodes->next->next->next->value)->id;	
	switch(element_ptr->group)
	{
		case 1:
			if(node_plus_id != 0)
			{
				if(x_node_plus_id != 0)
				{
					h_matrix[node_plus_id - 1][x_node_plus_id - 1] += value;
				}

				if(x_node_minus_id != 0)
				{
					h_matrix[node_plus_id - 1][x_node_minus_id - 1] += -value;
				}
			}
			
			if(node_minus_id != 0)
			{
				if(x_node_plus_id != 0)
				{
					h_matrix[node_minus_id - 1][x_node_plus_id - 1] += -value;
				}

				if(x_node_minus_id != 0)
				{
					h_matrix[node_minus_id - 1][x_node_minus_id - 1] += value;
				}
			}

			break;

		case 2:
			;		
			int current_id = extra_currents_positions[element_ptr->id];
			if(node_plus_id != 0)
			{
				h_matrix[node_plus_id - 1][current_id - 1] += 1;
			}

			if(node_minus_id != 0)
			{
				h_matrix[node_minus_id - 1][current_id - 1] += -1;
			}

			if(x_node_plus_id != 0)
			{
				h_matrix[current_id - 1][x_node_plus_id - 1] += -value;
			}

			if(x_node_minus_id != 0)
			{
				h_matrix[current_id - 1][x_node_minus_id - 1] += value;
			}

			h_matrix[current_id - 1][current_id - 1] += 1;
		
			break;
	}	
}


void list_element_groups(int position, element* element_ptr, void* additional_data)
{
	printf("Element: %s => Group %d; Extra current index %d\n", element_ptr->name, element_ptr->group, extra_currents_positions[position]);
}

void print_matrices()
{
	printf("\n");
	int i, j;
	for(i = 0; i < matrix_dim; i++)
	{
		for(j = 0; j < matrix_dim; j++)
		{
			printf("H(%d,%d) = %le\n", i + 1, j + 1, h_matrix[i][j]);
		}
	}

	printf("\n");
	for(i = 0; i < matrix_dim; i++)
	{
		printf("B(%d) = %le\n", i + 1, b_vector[i]);
	}
}
