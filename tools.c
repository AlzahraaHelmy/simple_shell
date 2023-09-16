#include "shell.h"

/**
 * interactivationmode - Returns true
 * @info:  address
 * Return: 1 or 0
 */
int interactivationmode(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * is_adelim - Checks if is specified
 * @c:  to check
 * @delim: delim
 * Return: 1 or 0
 */
int is_adelim(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}

/**
 *_isalpha - checks for  char
 *@c:  character to input
 *Return: 1 or 0
 */

int _isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 *_astoi - Converts to an integer
 *@s: inout
 *Return: 0
 */

int _astoi(char *s)
{
	int i, sin = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0;  s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sin *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sin == -1)
		output = -result;
	else
		output = result;

	return (output);
}

