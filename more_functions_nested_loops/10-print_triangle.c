#include "main.h"

/**
 * print_triangle - prints a triangle, followed by a new line
 * @size: the size of the triangle
 *
 * Return: void
 */
void print_triangle(int size)
{
	int i;
	int j;

	if (size <= 0)
		_putchar(10);
	for (i = 1; i <= size; i++)
	{
		for (j = 0; j < size - i; j++)
			_putchar(32);
		for (j = 0; j < i; j++)
			_putchar(35);
		_putchar(10);
	}
}
