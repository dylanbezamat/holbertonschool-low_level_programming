#include "main.h"

/**
 * _strspn - gets the length of a prefix substring
 * @s: the string to scan
 * @accept: the set of accepted bytes
 *
 * Return: the number of bytes in the initial segment of s
 * which consist only of bytes from accept
 */
unsigned int _strspn(char *s, char *accept)
{
	unsigned int i;
	int j;

	for (i = 0; s[i] != '\0'; i++)
	{
		for (j = 0; accept[j] != '\0'; j++)
		{
			if (s[i] == accept[j])
				break;
		}
		if (accept[j] == '\0')
			return (i);
	}
	return (i);
}
