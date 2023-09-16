#include "shell.h"

/**
 * clear_inf - initializes inf
 * @info: address
 * Return : void
 */
void clear_inf(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_inf - initializes info_t struct
 * @info: address
 * @av: argument
 * Return : void
 */
void set_inf(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		rep_alias(info);
		rep_vars(info);
	}
}

/**
 * rm_info - Edits info_t
 * @info: address
 * @all: True if all fields are edited
 * Return : void
 */
void rm_info(info_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			free_the_list(&(info->env));
		if (info->history)
			free_the_list(&(info->history));
		if (info->alias)
			free_the_list(&(info->alias));
		ffree(info->environ);
			info->environ = NULL;
		bbfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
