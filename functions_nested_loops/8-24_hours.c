#include "main.h"

/**
 * jack_bauer - prints every minute of the day
 *
 * Return: void
 */
void jack_bauer(void)
{
	int h;
	int m;

	h = 0;
	while (h < 24)
	{
		m = 0;
		while (m < 60)
		{
			_putchar(h / 10 + 48);
			_putchar(h % 10 + 48);
			_putchar(58);
			_putchar(m / 10 + 48);
			_putchar(m % 10 + 48);
			_putchar(10);
			m++;
		}
		h++;
	}
}
