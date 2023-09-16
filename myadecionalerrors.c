#include "shell.h"

/**
 * _erroratoi - converts to an integer
 * @s: to be converted
 * Return: 0 or result or -1
 */
int _erroratoi(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;
	for (i = 0;  s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * print_erro - prints msg error
 * @info: return info struct
 * @estr: string
 * Return: 0 or -1
 */
void print_erro(info_t *info, char *estr)
{
	_eputses(info->fname);
	_eputses(": ");
	prnt_d(info->l_c, STDERR_FILENO);
	_eputses(": ");
	_eputses(info->argv[0]);
	_eputses(": ");
	_eputses(estr);
}

/**
 * prnt_d - The function prints a decimal (integer) number base ( 10)
 * @input: the input
 * @fd: the file
 * Return: num
 */
int prnt_d(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		__putchar = _eputseschars;
	if (input < 0)
	{
		_abs_ = -input;
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * conv_number - converter function
 * @num: Number
 * @base: Base
 * @flags: argument flags
 *
 * Return: string
 */
char *conv_number(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * remv_comments - function to uncomment
 * @buf: address of the string
 * Return: 0;
 */
void remv_comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}
