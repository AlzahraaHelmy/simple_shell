#include "shell.h"

/**
 * list_the_len - length of linked list
 * @h: pointer to first
 * Return: len
 */
size_t list_the_len(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * list_to_strs - returns
 * @head: pointer to first node
 * Return: array of strs
 */
char **list_to_strs(list_t *head)
{
	list_t *node = head;
	size_t i = list_the_len(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _strcpy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}

/**
 * print_the_list - prints all elements
 * @h: pointer
 * Return: size of list
 */
size_t print_the_list(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(conv_number(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * node_starting_with - returns node
 * @node: pointer to list
 * @prefix: string to match
 * @c: the next character
 * Return: match node or 0
 */
list_t *node_starting_with(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = starting_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * get_node_index - index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index or -1
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}
