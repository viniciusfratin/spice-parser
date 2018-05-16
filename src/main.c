#include "main.h"
#include "parser.h"
#include "lexer.h"
#include "label_list.h"
#include "element_list.h"
#include "spice_format.h"

void execute_command(int position, void* command_ptr, void* additional_data);
void start_mna();

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
	generic_list_initialize(&p_data.command_list);

	result = yyparse(scanner, &p_data);

	yylex_destroy(scanner);

	fclose(input);

	if(result == 0)
	{
		generic_list_enumerate(p_data.command_list, &execute_command, NULL);
	}

	generic_list_clear(&p_data.command_list);
	element_list_clear(&p_data.element_list);
	label_list_clear(&p_data.label_list);
	
	return result;
}

void execute_command(int position, void* command_ptr, void* additional_data)
{
	command cmd = *((command*)command_ptr);
	switch(cmd.type)
	{
		case COMMAND_OP:
			start_mna();
			break;

		case COMMAND_INVALID:
			fprintf(stderr, "Invalid command\n");
			exit(1);
			break;
	}
}

void start_mna()
{
	printf("STARTING MNA!\n");
}
