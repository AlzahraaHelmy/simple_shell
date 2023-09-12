#include"myshell.h"
void handleCtrlC(int signum) {
    signum++;
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
    int retrncode, status,isinteractive,counter;
    char* buffer, * tokens;
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
        pid = fork();
        if (pid == -1)
        {
            perror("Error:");
            free(buffer);
            return (1);
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
        else
        {
            wait(&status);
        }
        free(child_argv);
    }
    free(buffer);
    return (0);
}
