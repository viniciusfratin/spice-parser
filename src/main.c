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

	yylex_init(&scanner);

	struct lexer_data* data;
	yylex_init_extra(data, &scanner);   	
	yyset_in(input, scanner);

	label_list_initialize(&data->global_label_list);
	label_list_insert(&data->global_label_list, "0");
	label_list_initialize(&data->current_label_list);

	result = yyparse(scanner);

	yylex_destroy(scanner);

	fclose(input);
   	
	return result;
}
