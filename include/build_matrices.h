#ifndef BUILD_MATRICES_H_
#define BUILD_MATRICES_H_

#include "spice_format.h"

void set_nodes(int position, label* node, void* additional_data);
void check_non_linear_models(int position, element* element_ptr, void* additional_data);
void generate_non_linear_companions(int position, element* element_ptr, void* additional_data);
void generate_diode_companions(int position, element* element_ptr, void* additional_data);
void generate_mosfet_companions(int position, element* element_ptr, void* additional_data);
void set_companion_values(int position, element* element_ptr, void* additional_data);
void set_diode_companion_values(element* diode, double* x_vector);
void set_mosfet_companion_values(element* mosfet, double* x_vector);
void classify_element_groups(int position, element* element_ptr, void* additional_data);
void generate_element_stamps(int position, element* element_ptr, void* additional_data);
void generate_resistor_stamp(element* resistor);
void generate_current_source_stamp(element* element_ptr);
void generate_voltage_source_stamp(element* element_ptr);
void generate_vcv_source_stamp(element* element_ptr);
void generate_ccv_source_stamp(element* element_ptr);
void generate_ccc_source_stamp(element* element_ptr);
void generate_vcc_source_stamp(element* element_ptr);
void list_element_groups(int position, element* element_ptr, void* additional_data);
void list_element_params(int position, parameter* param, void* additional_data);

#endif
