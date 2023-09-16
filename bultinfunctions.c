#include "shell.h"

/**
 * _myhistro - displays the history list
 * @info: Structure
 *  Return: 0
 */
int _myhistro(info_t *info)
{
	print_the_list(info->history);
	return (0);
}

/**
 * unset_alias - sets alias to str
 * @info: parameter struct
 * @str: the string alias
 * Return: 0 or 1
 */
int unset_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = _stringchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_in_indx(&(info->alias),
		get_node_index(info->alias, node_starting_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to str
 * @info: parameter
 * @str: the string
 * Return: 1 or 0
 */
int set_alias(info_t *info, char *str)
{
	char *p;

	p = _stringchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (add_nod_to_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias str
 * @node: the alias
 * Return: 1 or 0
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _stringchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalia - the alias builtin
 * @info: Structure
 *  Return: 0
 */
int _myalia(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = _stringchr(info->argv[i], '=');
		if (p)
			set_alias(info, info->argv[i]);
		else
			print_alias(node_starting_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
