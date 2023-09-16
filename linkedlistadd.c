#include "shell.h"

/**
 * add_node - node to the start
 * @head: address
 * @str: str
 * @num: num
 * Return: len
 */
list_t *add_node(list_t **head, const char *str, int num)
{
	list_t *nhead;

	if (!head)
		return (NULL);
	nhead = malloc(sizeof(list_t));
	if (!nhead)
		return (NULL);
	_memmoset((void *)nhead, 0, sizeof(list_t));
	nhead->num = num;
	if (str)
	{
		nhead->str = _strdup(str);
		if (!nhead->str)
		{
			free(nhead);
			return (NULL);
		}
	}
	nhead->next = *head;
	*head = nhead;
	return (nhead);
}

/**
 * add_nod_to_end - adds a node
 * @head: address
 * @str: str field
 * @num: node index
 * Return: len of list
 */
list_t *add_nod_to_end(list_t **head, const char *str, int num)
{
	list_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);
	_memmoset((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;
	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * print_the_list_mystr - prints
 * @h: pointer
 * Return: size
 */
size_t print_the_list_mystr(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * delete_node_in_indx - deletes
 * @head: address
 * @index: index
 * Return: 1 or 0
 */
int delete_node_in_indx(list_t **head, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * free_the_list - frees all nodes
 * @head_ptr: address
 * Return: nothing
 */
void free_the_list(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}
