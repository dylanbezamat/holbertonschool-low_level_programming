#include <stdlib.h>
#include "main.h"

/**
 * _calloc - alloue un tableau et met la memoire a zero
 * @nmemb: nombre d'elements
 * @size: taille de chaque element en octets
 *
 * Return: pointeur vers la memoire, ou NULL en cas d'echec
 */
void *_calloc(unsigned int nmemb, unsigned int size)
{
	char *p;
	unsigned int i, total;

	if (nmemb == 0 || size == 0)
		return (NULL);

	total = nmemb * size;
	p = malloc(total);
	if (p == NULL)
		return (NULL);

	for (i = 0; i < total; i++)
		p[i] = 0;

	return (p);
}
