#include <ctype.h>
#include <string.h>
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
    		if(*i != ' ' && *i != '\t')
		{
      			i++;
		}
  	}
  		
	*i = 0;
	
	return source;
}

int lowercase_compare(const char* str1, const char* str2)
{
	char s1[512], s2[512];
	strcpy(s1, str1);
	strcpy(s2, str2);

	convert_to_lowercase(s1);
	convert_to_lowercase(s2);

	return strcmp(s1, s2);
}
