#include "main.h"

/**
 * cap_string - capitalizes all words of a string
 * @s: the string to modify
 *
 * Return: a pointer to s
 */
char *cap_string(char *s)
{
	char *sep = " \t\n,;.!?\"(){}";
	int i;
	int j;
	int cap;

	cap = 1;
	for (i = 0; s[i] != '\0'; i++)
	{
		if (cap && s[i] >= 'a' && s[i] <= 'z')
			s[i] = s[i] - ('a' - 'A');
		cap = 0;
		for (j = 0; sep[j] != '\0'; j++)
		{
			if (s[i] == sep[j])
				cap = 1;
		}
	}
	return (s);
}
