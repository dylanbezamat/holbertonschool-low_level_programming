#include <stdlib.h>
#include "main.h"

/**
 * array_range - cree un tableau d'entiers de min a max inclus
 * @min: valeur minimale
 * @max: valeur maximale
 *
 * Return: pointeur vers le tableau, ou NULL en cas d'echec
 */
int *array_range(int min, int max)
{
	int *arr;
	int i, size;

	if (min > max)
		return (NULL);

	size = max - min + 1;
	arr = malloc(sizeof(int) * size);
	if (arr == NULL)
		return (NULL);

	for (i = 0; i < size; i++)
		arr[i] = min + i;

	return (arr);
}
