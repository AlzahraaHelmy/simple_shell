#include "shell.h"

/**
 **_strincpy - copy
 *@dest: the dest
 *@src: source
 *@n: the amount of characters
 *Return: concat string
 */
char *_strincpy(char *dest, char *src, int n)
{
	int i, j;
	char *s = dest;

	i = 0;
	while (src[i] != '\0' && i < n - 1)
	{
		dest[i] = src[i];
		i++;
	}
	if (i < n)
	{
		j = i;
		while (j < n)
		{
			dest[j] = '\0';
			j++;
		}
	}
	return (s);
}

/**
 **_strincat - It connects two chains
 *@dest: first dtr
 *@src: second str
 *@n: The amount of bytes to be used to the maximum extent
 *Return: concat str
 */
char *_strincat(char *dest, char *src, int n)
{
	int i, j;
	char *s = dest;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0' && j < n)
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	if (j < n)
		dest[i] = '\0';
	return (s);
}

/**
 **_stringchr - Specifies the location of a char in a string
 *@s: the str
 *@c: the char
 *Return: a pointer char
 */
char *_stringchr(char *s, char c)
{
	do {
		if (*s == c)
			return (s);
	} while (*s++ != '\0');

	return (NULL);
}
