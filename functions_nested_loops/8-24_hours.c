#include "main.h"

/**
 * jack_bauer - prints every minute of the day, from 00:00 to 23:59
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
			_putchar(h / 10 + '0');
			_putchar(h % 10 +
sed -i 's/	/\t/g' 8-24_hours.c
cat 8-24_hours.c
betty 8-24_hours.c
printf '#include "main.h"\nint main(void)\n{\n\tjack_bauer();\n\treturn (0);\n}\n' > 8-main.c
gcc -Wall -pedantic -Werror -Wextra -std=gnu89 _putchar.c 8-main.c 8-24_hours.c -o 8-24
./8-24 | head -3
./8-24 | tail -2
./8-24 | wc -l
cd /root/holbertonschool-low_level_programming
git add functions_nested_loops/8-24_hours.c
git commit -m "Task 8: jack_bauer"
git push
cd /root/holbertonschool-low_level_programming
ls functions_nested_loops/
git status
cd /root/holbertonschool-low_level_programming/functions_nested_loops
cat > 8-24_hours.c << 'EOF'
#include "main.h"

/**
 * jack_bauer - prints every minute of the day, from 00:00 to 23:59
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
			_putchar(h / 10 + '0');
			_putchar(h % 10 + '0');
			_putchar(':');
			_putchar(m / 10 + '0');
			_putchar(m % 10 + '0');
			_putchar('\n');
			m++;
		}
		h++;
	}
}
