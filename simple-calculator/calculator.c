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
 * main - simple calculator menu loop
 *
 * Return: Always 0
 */
int main(void)
{
	int choice;
	int res;

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
	}
	printf("Bye!\n");
	return (0);
}
