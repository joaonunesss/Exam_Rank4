#include "microshell.h"

int ft_perror(char *str)
{
    while(*str)
        write(2, str++, 1);
    return (1);
}

int ft_cd(char **av, int i)
{
    if (i != 2)
        return (ft_perror(BAD));
    if (chdir(av[i]) == -1)
        return (ft_perror(PATH), ft_perror(av[i]), ft_perror("\n"));
    return (0);
}

int ft_exec(char **av, char **env, int i)
{
    int pid;
    int fd[2];
    int has_pipe = av[i] && !strcmp(av[i], "|");
    int g_exit;

    if(has_pipe && pipe(fd) == -1)
        return (ft_perror(FATAL));
    pid = fork();
    if (pid == 0)
    {
        av[i] = NULL;
        if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
            return (ft_perror(FATAL));
        execve(*av, av, env);
        return (ft_perror(EXEC), ft_perror(*av), ft_perror("\n"));
    }
    waitpid(pid, &g_exit, 0);
    if (has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
            return (ft_perror(FATAL));
    return (WEXITSTATUS(g_exit));
}

int main(int ac, char **av, char **env)
{
    int i = 0;
    int g_exit = 0;

    if (ac < 2)
        return (1);
    while (av[i] && av[++i])
    {
        av += i;
        i = 0;
        while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
            i++;
        if (!strcmp(*av, "cd"))
            g_exit = ft_cd(av, i);
        else if (i)
            g_exit = ft_exec(av, env, i);
    }
    return (g_exit);
}