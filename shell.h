#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

#define USE_getlne 0
#define USE_STRTOK 0

#define HIST_FILE	".my_shell_history"
#define HIST_MAX	1024

extern char **environ;


/**
 * struct liststr - singly
 * @num: the number
 * @str: a string
 * @next: point next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguement
 *@arg: a string generated from getline
 *@argv: an array of strings
 *@path: a string path for command
 *@argc: the argument
 *@l_c: the error
 *@num_of_errors: the error code for exit()s
 *@lc_flag: if on count this line input
 *@fname: the program's filename
 *@env: linked list local environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int l_c;
	int num_of_errors;
	int lc_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf;
	int cmd_buf_type;
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains string
 *@type: the builtin flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


int hsh(info_t *, char **);
int find_builin(info_t *);
void finding_cmd(info_t *);
void forking_cmd(info_t *);
int is_this_cmd(info_t *, char *);
char *dups_chars(char *, int, int);
char *finds_paths(info_t *, char *, char *);
int loobhsh(char **);
int _eputseschars(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);
void _eputses(char *);
int _strlen(char *);
int _strcmp(char *, char *);
char *starting_with(const char *, const char *);
char *_strcat(char *, char *);
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);
char *_strincpy(char *, char *, int);
char *_strincat(char *, char *, int);
char *_stringchr(char *, char);
char **strtow(char *, char *);
char **strtows(char *, char);
char *_memmoset(char *, char, unsigned int);
void ffree(char **);
void *_reallocacte(void *, unsigned int, unsigned int);
int bbfree(void **);
int interactivationmode(info_t *);
int is_adelim(char, char *);
int _isalpha(int);
int _astoi(char *);
int _erroratoi(char *);
void print_erro(info_t *, char *);
int prnt_d(int, int);
char *conv_number(long int, int, int);
void remv_comments(char *);
int _myexit(info_t *);
int _my_cd(info_t *);
int _my_hlp(info_t *);
int _myhistro(info_t *);
int _myalia(info_t *);
ssize_t get_inpo(info_t *);
int _getlne(info_t *, char **, size_t *);
void sigins_Handl(int);
void clear_inf(info_t *);
void set_inf(info_t *, char **);
void rm_info(info_t *, int);
int _my_env(info_t *);
int _myset_env(info_t *);
int _myunset_env(info_t *);
int popu_envo_list(info_t *);
char *_get_env(info_t *, const char *);
char **get_enviro(info_t *);
int _unset_env(info_t *, char *);
int _set_env(info_t *, char *, char *);
char *get_histo_file(info_t *info);
int wrt_history(info_t *info);
int rd_history(info_t *info);
int build_histrooy_list(info_t *info, char *buf, int linecount);
int renum_history(info_t *info);
list_t *add_node(list_t **, const char *, int);
list_t *add_nod_to_end(list_t **, const char *, int);
size_t print_the_list_mystr(const list_t *);
int delete_node_in_indx(list_t **, unsigned int);
void free_the_list(list_t **);
size_t list_the_len(const list_t *);
char **list_to_strs(list_t *);
size_t print_the_list(const list_t *);
list_t *node_starting_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);
int is_chain(info_t *, char *, size_t *);
void check_ch(info_t *, char *, size_t *, size_t, size_t);
int rep_alias(info_t *);
int rep_vars(info_t *);
int rep_string(char **, char *);
#endif
