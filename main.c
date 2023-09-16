#include"myshell.h"
ssize_t my_getline(char** lineptr, size_t* n, FILE* stream) {
    int c;
    size_t i = 0;
    if (*lineptr == NULL) {
        *n = INITIAL_BUFFER_SIZE;
        *lineptr = (char*)malloc(*n);
        if (*lineptr == NULL) {
            return -1;
        }
    }
    while ((c = fgetc(stream)) != EOF && c != '\n') {
        if (i >= *n - 1) {
            *n *= 2;
            char* new_ptr = (char*)realloc(*lineptr, *n);
            if (new_ptr == NULL) {
                return -1;
            }
            *lineptr = new_ptr;
        }

        (*lineptr)[i++] = (char)c;
    }

    if (i == 0 && c == EOF) {
        return -1;
    }

    (*lineptr)[i] = '\0';
    return i;
}
void print_env(char** envp)
{
    int len;
    char** env;
    for (env = envp; *env != NULL; env++)
    {
        len = 0;
        while ((*env)[len] != '\0')
            len++;
        write(1, *env, len + 1);
        write(1, "\n", 2);
    }
}
int handle_env(char* c, char** envp) {
    int len = 0;
    while (c[len] != '\0')
        len++;
    if (len == 3)
    {
        if (c[0] == 'e' && c[1] == 'n' && c[2] == 'v')
        {
            print_env(envp);
            return 1;
        }
    }
    return 0;
}
char* inttoa(int val, int base) {

    static char buf[32] = { 0 };

    int i = 30;

    for (; val && i; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i + 1];

}

void handle_exit(char* c, char** child_argv, int lasterror)
{
    int len = 0;
    while (c[len] != '\0')
        len++;
    if (len == 4)
    {
        if (c[0] == 'e' && c[1] == 'x' && c[2] == 'i' && c[3] == 't')
        {
            free(c);
            free(child_argv);
            if (lasterror == 0)
                exit(lasterror);
            else
                exit(2);
        }
    }
}
/**
 * write_char_in_error - writes the character c to stderr
 * @c: The character to print
 * Return: 1 or -1
 */
int write_char_in_error(char c)
{
    static int i;
    static char buf[1024];
    if (c == -1 || i >= 1024)
    {
        write(2, buf, i);
        i = 0;
    }
    if (c != -1)
        buf[i++] = c;
    return (1);
}
/**
 * write_in_error - prints an input
 * @str: the str
 * Return: Nothing
 */
void write_in_error(char* mystr)
{
    int i = 0;
    if (!mystr)
        return;
    while (mystr[i] != '\0')
    {
        write_char_in_error(mystr[i]);
        i++;
    }
}
void write_error_message(char* argv, int loopcount, char* commd)
{
    write_in_error(argv);
    write_in_error(": ");
    write_in_error(inttoa(loopcount, 10));
    write_in_error(": ");
    write_in_error(commd);
    write_in_error(": not found");
    write_char_in_error('\n');
    write_char_in_error(-1);
}
void handleCtrlC(int signum) {
    signum++;
}
char* _strdub(const char* src)
{
    char* dest;
    int i, n;
    i = 0;
    n = 0;
    while (src[n] != '\0')
        n++;
    dest = (char*)calloc(n + 1, sizeof(char));
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    return (dest);
}
char* make_full_path(const char* start, const char* end) {
    int start_length, end_length, i;
    char* result;
    start_length = 0;
    end_length = 0;
    while (start[start_length] != '\0') {
        start_length++;
    }
    while (end[end_length] != '\0') {
        end_length++;
    }
    result = (char*)malloc(start_length + 1 + end_length + 1);
    if (result == NULL) {
        perror("Memory allocation error");
        return NULL;
    }
    for (i = 0; i < start_length; i++) {
        result[i] = start[i];
    }
    result[start_length] = '/';
    for (i = 0; i < end_length; i++) {
        result[start_length + 1 + i] = end[i];
    }
    result[start_length + 1 + end_length] = '\0';
    return result;
}
char* isCommandInPath(char* command, const char* path) {
    char* token;
    char* path_clone = _strdub(path);
    char* full_path;
    if (command[0] == '/' || command[0] == '.')
    {
        free(path_clone);
        full_path = _strdub(command);
        return full_path;
    }
    if (path_clone == NULL) {
        perror("Memory allocation error");
        exit(-1);
    }
    token = strtok(path_clone, ":");

    while (token != NULL) {
        full_path = make_full_path(token, command);
        if (access(full_path, X_OK) == 0) {
            free(path_clone);
            return full_path;
        }
        token = strtok(NULL, ":");
        free(full_path);
    }
    free(path_clone);
    return NULL;
}
char* extractPathFromEnv(char* envp[]) {
    char* path = NULL;
    int i;
    for (i = 0; envp[i] != NULL; i++) {
        if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T' && envp[i][3] == 'H' && envp[i][4] == '=') {
            path = envp[i] + 5;
            break;
        }
    }
    return path;
}
void remove_newline_at_end(char** buffer, ssize_t* size_read)
{
    if (*(*buffer + *size_read - 1) == '\n')
    {
        *(*buffer + *size_read - 1) = '\0';
        --size_read;
    }
}
int main(int argc, char* argv[], char* envp[])
{
    const char delim[2] = " ";
    pid_t pid;
    size_t buff_size;
    ssize_t size_read;
    int retrncode, status, isinteractive, counter, loopcount = 0, lasterror = 0, isenv;
    char* buffer = 0, * tokens, * path, *fullpath = 0;
    char** child_argv;
    buff_size = (size_t)MAX_INPUT_SIZE;
    signal(SIGINT, handleCtrlC);
    status = 0;
    /*buffer = (char*)malloc(MAX_INPUT_SIZE * sizeof(char));*/

    /*setbuf(stdout, NULL);*/
    if (argc > 2)
    {
        printf("%s", envp[0]);
        exit(0);
        /* todo imp;ement run from args*/
    }
    while (1) {
        loopcount++;
        pid = -2;
        isinteractive = isatty(fileno(stdin));
        if (isinteractive) {
            write(1, prompt, 3);
        }
        size_read = my_getline(&buffer, &buff_size, stdin);
        if (size_read == -1) {
            if (buffer != fullpath)
                free(buffer);
            return(lasterror);
        }
        remove_newline_at_end(&buffer, &size_read);
        tokens = strtok(buffer, delim);
        if (tokens == NULL)
            continue;
        counter = 0;
        child_argv = (char**)calloc(MAX_NUM_OF_ARGS, sizeof(char*));
        while (tokens != NULL)
        {
            child_argv[counter] = tokens;
            tokens = strtok(NULL, delim);
            counter++;
        }
        if (strcmp(child_argv[0], argv[0]) == 0 || strcmp(child_argv[0], "") == 0)
        {
            continue;
        }
        handle_exit(child_argv[0], child_argv, lasterror);
        isenv = handle_env(child_argv[0], envp);
        if (isenv)
        {
            free(child_argv);
            continue;
        }
        path = extractPathFromEnv(envp);
        if (path == 0)
        {
            fullpath = child_argv[0];
            if (access(fullpath, X_OK) != 0 || (child_argv[0][0] != '/')) {
                write_error_message(argv[0], loopcount, child_argv[0]);
                lasterror = errno = 127;
                continue;
            }
        }
        else
        {
            fullpath = isCommandInPath(child_argv[0], path);
        }
        if (fullpath == NULL)
        {
            write_error_message(argv[0], loopcount, child_argv[0]);
            lasterror = errno = 127;
        }
        else {
            child_argv[0] = fullpath;
            pid = fork();
            if (pid == -1)
            {
                perror("Error:");
                free(buffer);
                return (1);
            }
        }
        if (pid == 0) {
            retrncode = execve(child_argv[0], child_argv, envp);
            if (retrncode == -1)
            {
                perror(argv[0]);
                free(buffer);
                exit(retrncode);
            }
            return retrncode;
        }
        else if (pid != -2)
        {
            wait(&status);
            lasterror = status;
        }
        free(fullpath);
        free(child_argv);
    }
    free(buffer);
    return (lasterror);
}
