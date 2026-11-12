#include <stdlib.h>
#include "dog.h"

/**
 * free_dog - libere un chien
 * @d: pointeur vers le chien a liberer
 *
 * Return: rien
 */
void free_dog(dog_t *d)
{
	if (d == NULL)
		return;

	free(d->name);
	free(d->owner);
	free(d);
}
