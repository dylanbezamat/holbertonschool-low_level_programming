#include <stdio.h>

/**
 * print_menu - prints the calculator menu
 */
void print_menu(void)
{
	printf("1) Add\n2) Subtract\n3) Multiply\n4) Divide\n0) Quit\n");
	printf("Choice: ");
}

/**
 * clear_input - discards the rest of the line typed by the user
 */
void clear_input(void)
{
	int c;

	c = getchar();
	while (c != 10 && c != EOF)
		c = getchar();
}

/**
 * read_number - asks for a number until the user types a valid one
 * @prompt: the text to print before reading
 *
 * Return: the number typed, or 0 if there is no more input
 */
int read_number(char *prompt)
{
	int n;
	int res;

	while (1)
	{
		printf("%s", prompt);
		res = scanf("%d", &n);
		if (res == EOF)
			return (0);
		clear_input();
		if (res == 1)
			return (n);
		printf("Invalid number\n");
	}
}

/**
 * main - simple calculator menu loop
 *
 * Return: Always 0
 */
int main(void)
{
	int choice;
	int res;
	int a;
	int b;

	printf("Simple Calculator\n");
	while (1)
	{
		print_menu();
		res = scanf("%d", &choice);
		if (res == EOF)
			break;
		clear_input();
		if (res != 1 || choice < 0 || choice > 4)
		{
			printf("Invalid choice\n");
			continue;
		}
		if (choice == 0)
			break;
		a = read_number("A: ");
		b = read_number("B: ");
		if (choice == 1)
			printf("Result: %d\n", a + b);
		else if (choice == 2)
			printf("Result: %d\n", a - b);
		else if (choice == 3)
			printf("Result: %d\n", a * b);
		else if (b == 0)
			printf("Error: division by zero\n");
		else
			printf("Result: %g\n", (double)a / b);
	}
	printf("Bye!\n");
	return (0);
}
