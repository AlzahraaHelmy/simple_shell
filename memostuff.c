#include "shell.h"

/**
 * bbfree - frees
 * @ptr: address
 * Return: 1 or 0.
 */
int bbfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
