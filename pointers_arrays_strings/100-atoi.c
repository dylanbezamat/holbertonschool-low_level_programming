#include "main.h"

/**
 * _atoi - converts a string to an integer
 * @s: the string to convert
 *
 * Return: the converted integer, or 0 if no number is found
 */
int _atoi(char *s)
{
	int i;
	int sign;
	int result;
	int started;

	i = 0;
	sign = 1;
	result = 0;
	started = 0;
	while (s[i] != '\0')
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result = result * 10 - (s[i] - '0');
			started = 1;
		}
		else if (started)
			break;
		else if (s[i] == '-')
			sign = -sign;
		i++;
	}
	if (sign == 1)
		result = -result;
	return (result);
}
