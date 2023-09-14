#include"myshell.h"
void handle_exit(char* c, char** child_argv)
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
            exit(0);
        }
    }
}
/**
 * _eputseschars - writes the character c to stderr
 * @c: The character to print
 * Return: 1 or -1
 */
int _eputseschars(char c)
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
 *_eputses - prints an input
 * @str: the str
 * Return: Nothing
 */
void _eputses(char* mystr)
{
    int i = 0;
    if (!mystr)
        return;
    while (mystr[i] != '\0')
    {
        _eputseschars(mystr[i]);
        i++;
    }
}
char* inttoa(int val, int base) {

    static char buf[32] = { 0 };

    int i = 30;

    for (; val && i; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i + 1];

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
    dest = (char*) calloc(n+1, sizeof(char));
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    return (dest);
}
char* make_full_path(const char* start, const char* end) {
    int start_length, end_length ,i;
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
    int retrncode, status,isinteractive,counter,loopcount = 0;
    char* buffer, * tokens, *path , *fullpath ;
    char** child_argv;
    buffer = (char*)malloc(MAX_INPUT_SIZE * sizeof(char));
    buff_size = (size_t)MAX_INPUT_SIZE;
    signal(SIGINT, handleCtrlC);
    setbuf(stdout, NULL);
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
        if (isinteractive) { /*runnig in interactive mode*/
            write(1, prompt, 10);
        }
        size_read = getline(&buffer, &buff_size, stdin);
        if (size_read == -1) {
            free(buffer);
            return(0);
        }
        remove_newline_at_end(&buffer, &size_read);
        tokens = strtok(buffer, delim);
        if (tokens == NULL)
            continue;
        counter = 0;
        child_argv = (char**)calloc(MAX_NUM_OF_ARGS , sizeof(char*));
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
        path = extractPathFromEnv(envp);
        if (path == 0)
        {
            perror("PATH not found");
            return(-1);
        }
        handle_exit(child_argv[0],child_argv);
        fullpath = isCommandInPath(child_argv[0], path);
        if (fullpath == NULL)
        {
            _eputses(argv[0]);
            _eputses(": ");
            _eputses(inttoa(loopcount, 10));
            _eputses(": ");
            _eputses(child_argv[0]);
            _eputses(": not found");
            _eputseschars('\n');
            _eputseschars(-1);

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
        /*my_pid = getpid();*/
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
        }
        free(fullpath);
        free(child_argv);
    }
    free(buffer);
    return (0);
}
