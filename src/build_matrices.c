#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "generic_list.h"
#include "parameter_list.h"
#include "element_list.h"
#include "mna.h"
#include "build_matrices.h"
#include "string_operations.h"

void set_nodes(int position, label* node, void* additional_data)
{
	if(node->id > 0)
	{
		matrix_pointers[node->id - 1] = node;
	}
}

void check_non_linear_models(int position, element* element_ptr, void* additional_data)
{
	switch(element_ptr->type)
	{
		case TYPE_DIODE:
			if(lowercase_compare(element_ptr->value.value.string_value, "DIODEM1") == 0)
			{
				element_ptr->model = MODEL_DIODEM1;

				if(parameter_list_contains_name(element_ptr->param_list, "IS", NULL) == -1)
				{
					parameter IS_param;
					strcpy(IS_param.name, "IS");

					// Default value.
					IS_param.value = 1e-12;

					parameter_list_insert(&element_ptr->param_list, IS_param);
				}

				if(parameter_list_contains_name(element_ptr->param_list, "N", NULL) == -1)
				{
					parameter N_param;
					strcpy(N_param.name, "N");
					
					// Default value.
					N_param.value = 1.5;
					
					parameter_list_insert(&element_ptr->param_list, N_param);
				}

				printf("Element: %s\n", element_ptr->name);
				printf("-> Using model: %s\n", element_ptr->value.value.string_value);
				parameter_list_enumerate(element_ptr->param_list, &list_element_params, NULL);
			}

			else
			{
				element_ptr->model = MODEL_INVALID;
				break;
			}
			
			break;

		case TYPE_MOSFET:
			if(lowercase_compare(element_ptr->value.value.string_value, "NMOS") == 0)
			{
				element_ptr->model = MODEL_NMOS;
			}

			else if(lowercase_compare(element_ptr->value.value.string_value, "PMOS") == 0)
			{
				element_ptr->model = MODEL_PMOS;
			}

			else
			{
				element_ptr->model = MODEL_INVALID;
				break;
			}

			if(parameter_list_contains_name(element_ptr->param_list, "K'", NULL) == -1)
			{
				parameter Kp_param;
				strcpy(Kp_param.name, "K'");

				// Default value.
				double default_value;
				switch(element_ptr->model)
				{
					case MODEL_NMOS:
						default_value = 110e-6;
						break;

					case MODEL_PMOS:
						default_value = 50e-6;
						break;
				}

				Kp_param.value = default_value;

				parameter_list_insert(&element_ptr->param_list, Kp_param);
			}

			if(parameter_list_contains_name(element_ptr->param_list, "VTH", NULL) == -1)
			{
				parameter VTH_param;
				strcpy(VTH_param.name, "VTH");

				// Default value.
				double default_value;
				switch(element_ptr->model)
				{
					case MODEL_NMOS:
						default_value = 0.7;
						break;

					case MODEL_PMOS:
						default_value = -0.7;
						break;
				}

				VTH_param.value = default_value;

				parameter_list_insert(&element_ptr->param_list, VTH_param);
			}

			if(parameter_list_contains_name(element_ptr->param_list, "LAMBDA", NULL) == -1)
			{
				parameter LAMBDA_param;
				strcpy(LAMBDA_param.name, "LAMBDA");

				// Default value.
				double default_value;
				switch(element_ptr->model)
				{
					case MODEL_NMOS:
						default_value = 0.04;
						break;

					case MODEL_PMOS:
						default_value = 0.04;
						break;
				}

				LAMBDA_param.value = default_value;

				parameter_list_insert(&element_ptr->param_list, LAMBDA_param);
			}

			if(parameter_list_contains_name(element_ptr->param_list, "W", NULL) == -1)
			{
				parameter W_param;
				strcpy(W_param.name, "W");

				// Default value.
				double default_value;
				switch(element_ptr->model)
				{
					case MODEL_NMOS:
						default_value = 5e-6;
						break;

					case MODEL_PMOS:
						default_value = 5e-6;
						break;
				}

				W_param.value = default_value;

				parameter_list_insert(&element_ptr->param_list, W_param);
			}

			if(parameter_list_contains_name(element_ptr->param_list, "L", NULL) == -1)
			{
				parameter L_param;
				strcpy(L_param.name, "L");

				// Default value.
				double default_value;
				switch(element_ptr->model)
				{
					case MODEL_NMOS:
						default_value = 1e-6;
						break;

					case MODEL_PMOS:
						default_value = 1e-6;
						break;
				}

				L_param.value = default_value;

				parameter_list_insert(&element_ptr->param_list, L_param);
			}

			break;
	}

	if(element_ptr->model == MODEL_INVALID)
	{
		printf("Invalid model %s for element %s\n", element_ptr->value.value.string_value, element_ptr->name);
		exit(1);
	}
}

void generate_non_linear_companions(int position, element* element_ptr, void* additional_data)
{
	switch(element_ptr->type)
	{
		case TYPE_DIODE:
			generate_diode_companions(position, element_ptr, additional_data);
			break;		

		case TYPE_MOSFET:
			generate_mosfet_companions(position, element_ptr, additional_data);
			break;
	}
}

void generate_diode_companions(int position, element* element_ptr, void* additional_data)
{
	switch(element_ptr->model)
	{
		case MODEL_DIODEM1:
			;
			element_list* elements = *((element_list**)additional_data);

			char* model = element_ptr->value.value.string_value;
			label* node_plus = (label*)element_ptr->nodes->value;
			label* node_minus = (label*)element_ptr->nodes->next->value;
 

			element diode_c_source;
					
			diode_c_source.type = TYPE_C_SOURCE;
	
			char c_source_name[256];
			sprintf(c_source_name, "$$$%s_C_SOURCE", element_ptr->name);
			strcpy(diode_c_source.name, c_source_name);
			diode_c_source.is_set = 1;
			generic_list_initialize(&diode_c_source.nodes);
			generic_list_insert(&diode_c_source.nodes, (void*)node_plus);
			generic_list_insert(&diode_c_source.nodes, (void*)node_minus);

			element_value c_value;
			c_value.is_numeric = 1;
			c_value.value.numeric_value = 10; // TODO
			diode_c_source.value = c_value;

			diode_c_source.param_list = NULL;


			element diode_resistance;
							
			diode_resistance.type = TYPE_RESISTOR;
			
			char resistance_name[256];
			sprintf(resistance_name, "$$$%s_RESISTOR", element_ptr->name);
			strcpy(diode_resistance.name, resistance_name);
			diode_resistance.is_set = 1;
			generic_list_initialize(&diode_resistance.nodes);
			generic_list_insert(&diode_resistance.nodes, (void*)node_plus);
			generic_list_insert(&diode_resistance.nodes, (void*)node_minus);
		
			element_value r_value;
			r_value.is_numeric = 1;
			r_value.value.numeric_value = 10; // TODO
			diode_resistance.value = r_value;
		
			diode_resistance.param_list = NULL;
		
		
			element_list_insert(&elements, diode_c_source);
			element_list_insert(&elements, diode_resistance);
		
			element* c_element;
			element* r_element;
			element_list_contains_name(elements, c_source_name, &c_element);
			element_list_contains_name(elements, resistance_name, &r_element);
		
			generic_list_insert(&element_ptr->companion_elements, (void*)c_element);
			generic_list_insert(&element_ptr->companion_elements, (void*)r_element);
	
			printf("-> Element: %s\n", element_ptr->name);
			printf("---> Generated element: %s\n", c_source_name);
			printf("---> Generated element: %s\n", resistance_name);
			
			break;
		
		default:
			printf("Invalid model\n");
			exit(1);
	}
}

void generate_mosfet_companions(int position, element* element_ptr, void* additional_data)
{
	switch(element_ptr->model)
	{
		case MODEL_NMOS:
			;
			element_list* elements = *((element_list**)additional_data);

			char* model = element_ptr->value.value.string_value;
			label* node_d = (label*)element_ptr->nodes->value;
			label* node_g = (label*)element_ptr->nodes->next->value;
			label* node_s = (label*)element_ptr->nodes->next->next->value;


			element mosfet_c_source;
					
			mosfet_c_source.type = TYPE_C_SOURCE;
	
			char c_source_name[256];
			sprintf(c_source_name, "$$$%s_C_SOURCE", element_ptr->name);
			strcpy(mosfet_c_source.name, c_source_name);
			mosfet_c_source.is_set = 1;
			generic_list_initialize(&mosfet_c_source.nodes);
			generic_list_insert(&mosfet_c_source.nodes, (void*)node_d);
			generic_list_insert(&mosfet_c_source.nodes, (void*)node_s);

			element_value c_value;
			c_value.is_numeric = 1;
			c_value.value.numeric_value = 10; // TODO
			mosfet_c_source.value = c_value;

			mosfet_c_source.param_list = NULL;


			element mosfet_resistance;
							
			mosfet_resistance.type = TYPE_RESISTOR;
			
			char resistance_name[256];
			sprintf(resistance_name, "$$$%s_RESISTOR", element_ptr->name);
			strcpy(mosfet_resistance.name, resistance_name);
			mosfet_resistance.is_set = 1;
			generic_list_initialize(&mosfet_resistance.nodes);
			generic_list_insert(&mosfet_resistance.nodes, (void*)node_d);
			generic_list_insert(&mosfet_resistance.nodes, (void*)node_s);
		
			element_value r_value;
			r_value.is_numeric = 1;
			r_value.value.numeric_value = 10; // TODO
			mosfet_resistance.value = r_value;
		
			mosfet_resistance.param_list = NULL;
		
		
			element mosfet_vccs;
							
			mosfet_vccs.type = TYPE_VCC_SOURCE;
			
			char vccs_name[256];
			sprintf(vccs_name, "$$$%s_VCCS", element_ptr->name);
			strcpy(mosfet_vccs.name, vccs_name);
			mosfet_vccs.is_set = 1;
			generic_list_initialize(&mosfet_vccs.nodes);
			generic_list_insert(&mosfet_vccs.nodes, (void*)node_d);
			generic_list_insert(&mosfet_vccs.nodes, (void*)node_s);
			generic_list_insert(&mosfet_vccs.nodes, (void*)node_g);
			generic_list_insert(&mosfet_vccs.nodes, (void*)node_s);
		
			element_value vccs_value;
			vccs_value.is_numeric = 1;
			vccs_value.value.numeric_value = 10; // TODO
			mosfet_vccs.value = vccs_value;
		
			mosfet_vccs.param_list = NULL;


			element_list_insert(&elements, mosfet_c_source);
			element_list_insert(&elements, mosfet_resistance);
			element_list_insert(&elements, mosfet_vccs);
		
			element* c_element;
			element* r_element;
			element* vccs_element;
			element_list_contains_name(elements, c_source_name, &c_element);
			element_list_contains_name(elements, resistance_name, &r_element);
			element_list_contains_name(elements, vccs_name, &vccs_element);
		
			generic_list_insert(&element_ptr->companion_elements, (void*)c_element);
			generic_list_insert(&element_ptr->companion_elements, (void*)r_element);
			generic_list_insert(&element_ptr->companion_elements, (void*)vccs_element);
	
			printf("-> Element: %s\n", element_ptr->name);
			printf("---> Generated element: %s\n", c_source_name);
			printf("---> Generated element: %s\n", resistance_name);
			printf("---> Generated element: %s\n", vccs_name);
			
			break;
		
		default:
			printf("Invalid model\n");
			exit(1);
	}
}


void set_companion_values(int position, element* element_ptr, void* additional_data)
{
	double* x_vector = (double*)additional_data;
	switch(element_ptr->type)
	{
		case TYPE_DIODE:
			set_diode_companion_values(element_ptr, x_vector);
			break;

		case TYPE_MOSFET:
			set_mosfet_companion_values(element_ptr, x_vector);
			break;
	}
}

void set_diode_companion_values(element* diode, double* x_vector)
{
	element* first_elem = (element*)diode->companion_elements->value;
	element* second_elem = (element*)diode->companion_elements->next->value;

	element* c_source = first_elem->type == TYPE_C_SOURCE ? first_elem : second_elem;
	element* resistance = first_elem->type == TYPE_RESISTOR ? first_elem : second_elem;

	parameter* IS_param;
	parameter* N_param;

	parameter_list_contains_name(diode->param_list, "IS", &IS_param);
	parameter_list_contains_name(diode->param_list, "N", &N_param);

	int node_plus_id = ((label*)diode->nodes->value)->id;
	int node_minus_id = ((label*)diode->nodes->next->value)->id;

	double v_plus = node_plus_id == 0 ? 0 : x_vector[node_plus_id - 1];
	double v_minus = node_minus_id == 0 ? 0 : x_vector[node_minus_id - 1];
	double IS = IS_param->value;
	double N = N_param->value;
	double VT = 25.85e-3;
	double vd = v_plus - v_minus;

	double g_eq = (IS / (N * VT)) * exp(vd / (N * VT));
	double r_eq = 1/g_eq;
	double i_eq = IS * (exp(vd / (N * VT)) - 1) - vd * g_eq;
	
	c_source->value.value.numeric_value = i_eq;
	resistance->value.value.numeric_value = r_eq;
	printf("\n-> %s: %s = %le; %s = %le\n", diode->name, "r_eq", r_eq, "i_eq", i_eq);
}

void set_mosfet_companion_values(element* mosfet, double* x_vector)
{
	element* first_elem = (element*)mosfet->companion_elements->value;
	element* second_elem = (element*)mosfet->companion_elements->next->value;
	element* third_elem = (element*)mosfet->companion_elements->next->next->value;	

	element* c_source;
	element* resistance;
	element* vccs;
	if(first_elem->type == TYPE_C_SOURCE)
	{
		c_source = first_elem;
		resistance = second_elem->type == TYPE_RESISTOR ? second_elem : third_elem;
		vccs = second_elem->type == TYPE_VCC_SOURCE ? second_elem : third_elem;
	}

	if(second_elem->type == TYPE_C_SOURCE)
	{
		c_source = second_elem;
		resistance = first_elem->type == TYPE_RESISTOR ? first_elem : third_elem;
		vccs = first_elem->type == TYPE_VCC_SOURCE ? first_elem : third_elem;
	}

	if(third_elem->type == TYPE_C_SOURCE)
	{
		c_source = third_elem;
		resistance = first_elem->type == TYPE_RESISTOR ? first_elem : second_elem;
		vccs = first_elem->type == TYPE_VCC_SOURCE ? first_elem : second_elem;
	}

	parameter* Kp_param;
	parameter* VTH_param;
	parameter* LAMBDA_param;
	parameter* W_param;
	parameter* L_param;

	parameter_list_contains_name(mosfet->param_list, "K'", &Kp_param);
	parameter_list_contains_name(mosfet->param_list, "VTH", &VTH_param);
	parameter_list_contains_name(mosfet->param_list, "LAMBDA", &LAMBDA_param);
	parameter_list_contains_name(mosfet->param_list, "W", &W_param);
	parameter_list_contains_name(mosfet->param_list, "L", &L_param);

	int node_d_id = ((label*)mosfet->nodes->value)->id;
	int node_g_id = ((label*)mosfet->nodes->next->value)->id;
	int node_s_id = ((label*)mosfet->nodes->next->next->value)->id;

	double v_d = node_d_id == 0 ? 0 : x_vector[node_d_id - 1];
	double v_g = node_g_id == 0 ? 0 : x_vector[node_g_id - 1];
	double v_s = node_s_id == 0 ? 0 : x_vector[node_s_id - 1];
	double Kp = Kp_param->value;
	double VTH = VTH_param->value;
	double LAMBDA = LAMBDA_param->value;
	double W = W_param->value;
	double L = L_param->value;
	
	double v_ds = v_d - v_s;
	double v_gs = v_g - v_s;

	//double g_eq = (IS / (N * VT)) * exp(vd / (N * VT));
	//double r_eq = 1/g_eq;
	//double i_eq = IS * (exp(vd / (N * VT)) - 1) - vd * g_eq;
	
	//c_source->value.value.numeric_value = i_eq;
	//resistance->value.value.numeric_value = r_eq;
	//printf("\n-> %s: %s = %le; %s = %le\n", mosfet->name, "r_eq", r_eq, "i_eq", i_eq);
}


void classify_element_groups(int position, element* element_ptr, void* additional_data)
{
	element_list* elements = *((element_list**)additional_data);
	// Default.
	element_ptr->group = 1;

	switch(element_ptr->type)
	{
		case TYPE_CCV_SOURCE:
			element_ptr->group = 2;
			matrix_pointers[number_of_extra_currents + number_of_nodes - 1] = element_ptr;
			number_of_extra_currents++;
			extra_currents_positions[position] = number_of_extra_currents + number_of_nodes - 1;

		case TYPE_CCC_SOURCE:
			;
			element* ref_elem = (element*)element_ptr->ref_elements->value;
			ref_elem->group = 2;
			matrix_pointers[number_of_extra_currents + number_of_nodes - 1] = ref_elem;
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
			matrix_pointers[number_of_extra_currents + number_of_nodes - 1] = element_ptr;
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
	printf("Generating stamps for element: %s\n", element_ptr->name);
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

		case TYPE_DIODE:	
		case TYPE_MOSFET:
			break;

		default:
			fprintf(stderr, "Unsupported element %s.\n", element_ptr->name);
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
	
	if(element_ptr->value.is_numeric == 0)
	{
		printf("-> Model: %s\n", element_ptr->value.value.string_value);
		parameter_list_enumerate(element_ptr->param_list, &list_element_params, NULL);
	}
}

void list_element_params(int position, parameter* param, void* additional_data)
{
	printf("---> Parameter: %s = %le\n", param->name, param->value);
}
