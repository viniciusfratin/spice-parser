#include "main.h"
#include "parser.h"
#include "lexer.h"
#include "label_list.h"
#include "element_list.h"
#include "spice_format.h"
#include "mna.h"

void print_label(int label_index, label l, void* additional_data);
void print_element(int element_index, element* element_ptr, void* additional_data);
void print_node(int label_index, void* label_ptr, void* type_int_ptr);
void execute_command(int position, void* command_ptr, void* additional_data);

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "Please inform the input file name.\n");
		exit(1);
	}
	
	FILE *input;
	input = fopen(argv[1], "r");
	if(input == NULL)
	{
		fprintf(stderr, "Invalid file.\n");
		exit(1);
	}

	label_list* labels;
	element_list* elements;
	generic_list* commands;
	
	label_list_initialize(&labels);
	label_list_insert(&labels, "0");
	element_list_initialize(&elements);
	generic_list_initialize(&commands);

	int result;

	yyscan_t scanner;
	struct parser_data p_data;
 
	p_data.label_list = &labels;
	p_data.element_list = &elements;
	p_data.command_list = &commands;

	yylex_init(&scanner);

	yyset_in(input, scanner);
	
	result = yyparse(scanner, &p_data);

	yylex_destroy(scanner);

	fclose(input);

	if(result == 0)
	{	
		mna_data data;
		data.nodes = labels;
		data.elements = elements;

		generic_list_enumerate(commands, &execute_command, (void*)&data);
	}

	generic_list_clear(&commands);
	element_list_clear(&elements);
	label_list_clear(&labels);
	
	return result;
}


void print_element(int element_index, element* element_ptr, void* additional_data)
{
	element elem = *element_ptr;

	int id = elem.id;
	int type = elem.type;
	char type_name[64];
	get_element_type_name(type_name, type);
	label_list* nodes = elem.nodes;
	element_value value = elem.value;

	printf("#%d: %s [%s]", id + 1, type_name, elem.name);

	generic_list_enumerate(nodes, &print_node, (void*)&type);

	printf("\n\tValue = ");
	if(value.is_numeric)
	{
		printf("%.4le\n\n", value.value.numeric_value);
	}

	else
	{
		printf("%s\n\n", value.value.string_value);
	}
}

void print_node(int label_index, void* label_ptr, void* type_int_ptr)
{
	int type = *((int*)type_int_ptr);;

	char two_node_terminal_names[2] = {'+', '-'};
	char tjb_terminal_names[3] = {'C', 'B', 'E'};
	char mosfet_terminal_names[3] = {'D', 'G', 'S'};
	char cc_sources_terminal_names[3] = {'+', '-', 'e'};
	char four_node_terminal_names[4][3] = {"+", "-", "C+", "C-"};
 
	printf("\n\tNode #%d: ", label_index + 1);

	switch(type)
	{
		case TYPE_RESISTOR:
		case TYPE_CAPACITOR:
		case TYPE_INDUCTOR:
		case TYPE_V_SOURCE:
		case TYPE_C_SOURCE:
		case TYPE_DIODE:
			printf("n%c", 
				two_node_terminal_names[label_index]);
			break;		

		case TYPE_TJB:
			printf("n%c", 
				tjb_terminal_names[label_index]);
			break;	

		case TYPE_MOSFET:
			printf("n%c", 
				mosfet_terminal_names[label_index]);
			break;		
	
		case TYPE_CCC_SOURCE:
		case TYPE_CCV_SOURCE:
			printf("%c",
				cc_sources_terminal_names[label_index]);
			break;

		case TYPE_VCV_SOURCE:
		case TYPE_VCC_SOURCE:
			printf("n%s", 
				four_node_terminal_names[label_index]);
			break;		
	}

	label l = *((label*)label_ptr);
	printf("[%d]", l.id);
}

void execute_command(int position, void* command_ptr, void* additional_data)
{
	command cmd = *((command*)command_ptr);
	switch(cmd.type)
	{
		case COMMAND_OP:
			start_mna(*((mna_data*)additional_data));
			break;

		case COMMAND_INVALID:
			fprintf(stderr, "Invalid command\n");
			exit(1);
			break;
	}
}


