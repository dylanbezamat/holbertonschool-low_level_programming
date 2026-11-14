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
 * main - simple calculator skeleton
 *
 * Return: Always 0
 */
int main(void)
{
	int choice;

	printf("Simple Calculator\n");
	print_menu();
	if (scanf("%d", &choice) == 1 && choice == 0)
		printf("Bye!\n");
	return (0);
}
