#include "main.h"
#include "parser.h"
#include "lexer.h"
#include "label_list.h"
#include "element_list.h"
#include "spice_format.h"

struct node_info_struct
{
	int device_type;
	label_list* global_label_list;
};

typedef struct node_info_struct node_info;

void print_element(int element_index, element elem, void* additional_data);
void print_node(int label_index, label l, void* node_info_ptr);

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

	int result;

	yyscan_t scanner;
	struct parser_data p_data;
 
	yylex_init(&scanner);

	yyset_in(input, scanner);

	label_list_initialize(&p_data.label_list);
	label_list_insert(&p_data.label_list, "0");
	element_list_initialize(&p_data.element_list);

	result = yyparse(scanner, &p_data);

	yylex_destroy(scanner);

	fclose(input);

	printf("Circuit elements:\n");
	element_list_enumerate(p_data.element_list, &print_element, (void*)&p_data.label_list);
	return result;
}

void print_element(int element_index, element elem, void* additional_data)
{
	int id = elem.id;
	int type = elem.type;
	char type_name[64];
	get_element_type_name(type_name, type);
	label_list* nodes = elem.nodes;
	element_value value = elem.value;

	printf("#%d: %s [%s]", id + 1, type_name, elem.name);

	node_info n;
	n.device_type = type;
	n.global_label_list = *((label_list**)additional_data);	
	label_list_enumerate(nodes, &print_node, (void*)&n);

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

void print_node(int label_index, label l, void* node_info_ptr)
{
	node_info n = *((node_info*)node_info_ptr);
	int type = n.device_type;

	char two_node_terminal_names[2] = {'+', '-'};
	char tjb_terminal_names[3] = {'C', 'B', 'E'};
	char mosfet_terminal_names[3] = {'D', 'G', 'S'};
	char four_node_terminal_names[4][2] = {"+", "-", "C+", "C-"};
 
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
	
		case TYPE_VCV_SOURCE:
		case TYPE_CCC_SOURCE:
		case TYPE_VCC_SOURCE:
		case TYPE_CCV_SOURCE:
			printf("n%s", 
				four_node_terminal_names[label_index]);
			break;		
	}

	printf("[%d]", l.global_id);
}


