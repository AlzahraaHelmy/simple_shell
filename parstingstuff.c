#include "shell.h"

/**
 * is_this_cmd - if a file
 * @info: info struct
 * @path: path
 *
 * Return: true 1, 0
 */
int is_this_cmd(info_t *info, char *path)
{
	struct stat st;

	(void)info;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dups_chars - the duplicates chars
 * @pathstr: PATH string
 * @start: start
 * @stop: stopp
 * Return: pointing to new buffer
 */
char *dups_chars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i];
	buf[k] = 0;
	return (buf);
}

/**
 * finds_paths - finds this cmd
 * @info: the info struct
 * @pathstr: the PATH
 * @cmd: the cmd
 * Return: full path of cmd if found
 */
char *finds_paths(info_t *info, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && starting_with(cmd, "./"))
	{
		if (is_this_cmd(info, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dups_chars(pathstr, curr_pos, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (is_this_cmd(info, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
