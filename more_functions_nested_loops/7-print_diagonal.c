#include "main.h"

/**
 * print_diagonal - draws a diagonal line on the terminal
 * @n: the number of times the backslash character should be printed
 *
 * Return: void
 */
void print_diagonal(int n)
{
	int i;
	int j;

	if (n <= 0)
		_putchar(10);
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < i; j++)
			_putchar(32);
		_putchar(92);
		_putchar(10);
	}
}
