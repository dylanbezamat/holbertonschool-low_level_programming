#include <stdlib.h>

/**
 * malloc_checked - alloue de la mémoire avec malloc
 * @b: nombre d'octets à allouer
 *
 * Return: pointeur vers la mémoire allouée.
 * Si malloc échoue, le processus se termine avec le statut 98.
 */
void *malloc_checked(unsigned int b)
{
	void *p;

	p = malloc(b);
	if (p == NULL)
		exit(98);

	return (p);
}
