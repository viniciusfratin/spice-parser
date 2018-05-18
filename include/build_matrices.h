#ifndef BUILD_MATRICES_H_
#define BUILD_MATRICES_H_

#include "spice_format.h"

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

#endif
