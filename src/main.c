#include "main.h"
#include "parser.h"
#include "lexer.h"
#include "label_list.h"

#define BOOL char
#define TRUE 1
#define FALSE 0

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

	result = yyparse(scanner, &p_data);

	yylex_destroy(scanner);

	fclose(input);

	printf("\nnodes:\n");
	label_list_print(p_data.label_list);   	
	return result;
}
