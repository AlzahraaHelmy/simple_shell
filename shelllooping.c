#include "shell.h"

/**
 * hsh - shell loop
 * @info: the parameter info struct
 * @av: argument vector from main()
 * Return: 0 on success, 1 on error, error code
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_inf(info);
		if (interactivationmode(info))
			_puts("$ ");
		_eputseschars(BUF_FLUSH);
		r = get_inpo(info);
		if (r != -1)
		{
			set_inf(info, av);
			builtin_ret = find_builin(info);
			if (builtin_ret == -1)
				finding_cmd(info);
		}
		else if (interactivationmode(info))
			_putchar('\n');
		rm_info(info, 0);
	}
	wrt_history(info);
	rm_info(info, 1);
	if (!interactivationmode(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->num_of_errors == -1)
			exit(info->status);
		exit(info->num_of_errors);
	}
	return (builtin_ret);
}

/**
 * find_builin - finds a  command
 * @info: the parameter & return info struct
 *
 * Return: -1 or 0 or 1 or -2
 */
int find_builin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _my_env},
		{"help", _my_hlp},
		{"history", _myhistro},
		{"setenv", _myset_env},
		{"unsetenv", _myunset_env},
		{"cd", _my_cd},
		{"alias", _myalia},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->l_c++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * finding_cmd - finds a command
 * @info: parameter info struct
 *
 * Return: the void
 */
void finding_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->lc_flag == 1)
	{
		info->l_c++;
		info->lc_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_adelim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = finds_paths(info, _get_env(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		forking_cmd(info);
	}
	else
	{
		if ((interactivationmode(info) || _get_env(info, "PATH=")
			|| info->argv[0][0] == '/') && is_this_cmd(info, info->argv[0]))
			forking_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_erro(info, "not found\n");
		}
	}
}

/**
 * forking_cmd - forks a an exec thread
 * @info: return info struct
 *
 * Return: thr
 */
void forking_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_enviro(info)) == -1)
		{
			rm_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_erro(info, "Permission denied\n");
		}
	}
}
