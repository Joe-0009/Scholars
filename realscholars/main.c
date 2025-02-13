#include "philosophers.h"

int main(int ac, char **av)
{
    t_program program;
    if (check_args(ac, av))
    {
        exit(EXIT_FAILURE);
    }
    if (init_program_args(&program, ac, av))
    {
        exit(EXIT_FAILURE);
    }
    if (init_program(&program))
    {
        ft_putstr("Error: Failed to initialize program\n");
        return (1);
    }
    clean_program(&program);
    exit(EXIT_SUCCESS);
}