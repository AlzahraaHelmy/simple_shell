#include "shell.h"

/**
 * _myexit - close shell
 * @info: - Structure
 *  Return: 0 or 1 or -2
 */
int _myexit(info_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = _erroratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_erro(info, "Illegal number: ");
			_eputses(info->argv[1]);
			_eputseschars('\n');
			return (1);
		}
		info->num_of_errors = _erroratoi(info->argv[1]);
		return (-2);
	}
	info->num_of_errors = -1;
	return (-2);
}

/**
 * _my_cd - change directory
 * @info: Structure containing potential arguments.
 *
 *  Return: Always 0
 */
int _my_cd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("called cd\n");
	if (!info->argv[1])
	{
		dir = _get_env(info, "HOME=");
		if (!dir)
			chdir_ret =
				chdir((dir = _get_env(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!_get_env(info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_get_env(info, "OLDPWD=")), _putchar('\n');
		chdir_ret =
			chdir((dir = _get_env(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_erro(info, "can't cd to ");
		_eputses(info->argv[1]), _eputseschars('\n');
	}
	else
	{
		_set_env(info, "OLDPWD", _get_env(info, "PWD="));
		_set_env(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _my_hlp - help not emplemented
 * @info: potential arguments. Used to maintain
 *  Return: Always 0
 */
int _my_hlp(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("called the help \n");
	if (0)
		_puts(*arg_array);
	return (0);
}
