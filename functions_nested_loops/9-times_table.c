#include "main.h"

/**
 * times_table - prints the 9 times table, starting with 0
 *
 * Return: void
 */
void times_table(void)
{
	int i;
	int j;
	int r;

	i = 0;
	while (i < 10)
	{
		_putchar(48);
		j = 1;
		while (j < 10)
		{
			r = i * j;
			_putchar(44);
			_putchar(32);
			if (r < 10)
				_putchar(32);
			else
				_putchar(r / 10 + 48);
			_putchar(r % 10 + 48);
			j++;
		}
		_putchar(10);
		i++;
	}
}
