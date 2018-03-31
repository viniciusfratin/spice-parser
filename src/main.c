#include "y.tab.h"

#define BOOL char
#define TRUE 1
#define FALSE 0

BOOL must_register();

int main()
{
   yyparse();
   return 0;
}

BOOL must_register()
{
	if(line_counter == 1)
	{
		return FALSE;
	}
	
	else
	{
		return TRUE;
	}
}
