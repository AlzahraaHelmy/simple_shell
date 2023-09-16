#include "shell.h"

/**
 * get_enviro - returns the string array
 * @info: Structure
 * Return: 0
 */
char **get_enviro(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strs(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _unset_env - Remove an env
 * @info: Structure
 * @var: the string env var property
 *  Return: 1 or 0
 */
int _unset_env(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starting_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_in_indx(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * _set_env - Initialize a new env
 * @info: Structure
 * @var: the string env var
 * @value: value
 *  Return: 0
 */
int _set_env(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = info->env;
	while (node)
	{
		p = starting_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_nod_to_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
