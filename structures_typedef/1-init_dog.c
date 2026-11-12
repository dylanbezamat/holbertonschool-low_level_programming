#include <stdlib.h>
#include "dog.h"

/**
 * init_dog - initialise une variable de type struct dog
 * @d: pointeur vers la structure a initialiser
 * @name: nom du chien
 * @age: age du chien
 * @owner: proprietaire du chien
 *
 * Return: rien
 */
void init_dog(struct dog *d, char *name, float age, char *owner)
{
	if (d == NULL)
		return;

	d->name = name;
	d->age = age;
	d->owner = owner;
}
