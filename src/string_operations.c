#include <ctype.h>
#include "string_operations.h"

char* convert_to_lowercase(char* str)
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
	{
		str[i] = tolower(str[i]);
	}

	return str;
}

// From StackOverflow user Aaron.
char* remove_spaces(char* source)
{
	char* i = source;
  	char* j = source;
  	while(*j != 0)
  	{
    		*i = *j++;
    		if(*i != ' ')
		{
      			i++;
		}
  	}
  		
	*i = 0;
	
	return source;
}
