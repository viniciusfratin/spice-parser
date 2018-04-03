#include "main.h"
#include "parser.h"
#include "lexer.h"

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

	data->line_number = 1;

	result = yyparse(scanner);

	yylex_destroy(scanner);

	fclose(input);
   	
	return result;
}
