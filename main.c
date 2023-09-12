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
    int retrncode, status;
    char* buffer, * tokens;
    char* child_argv[] = { "/bin/ls", NULL };
    buffer = (char*)malloc(MAX_INPUT_SIZE * sizeof(char));
    buff_size = (size_t)MAX_INPUT_SIZE;
    signal(SIGINT, handleCtrlC);
    if (argc > 2)
    {
        printf("%s", envp[0]);
        exit(0);
        /* todo imp;ement run from args*/
    }
    while (1) {
        printf("$ ");
        size_read = getline(&buffer, &buff_size, stdin);
        if (size_read == -1) {
            printf("\n");/*exit the program*/
            exit(0);
        }
        remove_newline_at_end(&buffer, &size_read);
        tokens = strtok(buffer, delim);
        child_argv[0] = tokens;
        /*printf("You entered: '%s'\n", child_argv[0]);*/
        /*printf("child_argv[0]='%s'\n", child_argv[0]);*/
        /*printf("argv[0]='%s'\n", argv[0]); */
        if (strcmp(child_argv[0], argv[0]) == 0 || strcmp(child_argv[0], "") == 0)
        {
            /*printf("inside if");*/
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("Error:");
            return (1);
        }
        /*my_pid = getpid();*/
        if (pid == 0) {
            retrncode = execve(child_argv[0], child_argv, envp);
            if (retrncode == -1)
            {
                perror(argv[0]);
                exit(retrncode);
            }
            return retrncode;
        }
        else
        {
            wait(&status);

        }


    }
    return (0);
}
