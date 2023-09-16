#include "shell.h"

/**
 * _my_env - prints the current env
 * @info: Structure
 * Return: 0
 */
int _my_env(info_t *info)
{
	print_the_list_mystr(info->env);
	return (0);
}

/**
 * _get_env - gets the value
 * @info: Structure
 * @name: env name
 * Return: value
 */
char *_get_env(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = starting_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _myset_env - Initialize a new env
 * @info: Structure
 *  Return: 0
 */
int _myset_env(info_t *info)
{
	if (info->argc != 3)
	{
		_eputses("Incorrect number of arguements\n");
		return (1);
	}
	if (_set_env(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunset_env - Remove an env
 * @info: Structure
 *  Return: 0
 */
int _myunset_env(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		_eputses("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_unset_env(info, info->argv[i]);

	return (0);
}

/**
 * popu_envo_list - populates env ll
 * @info: Structure
 * Return: 0
 */
int popu_envo_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_nod_to_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}
