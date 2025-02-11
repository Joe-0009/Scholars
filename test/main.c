
#include "philosophers.h"


static void ft_putstr(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
}

static int	ft_atoi(const char *str)
{
    long	result;
    int		sign;
    
    result = 0;
    sign = 1;
    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        if (result * sign > INT_MAX || result * sign < INT_MIN)
            return (0);
        str++;
    }
    return (result * sign);
}

static int is_number(char *str)
{
    int i;

    i = 0;
    if (str[i] == '+')
        i++;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int init_program_args(t_program *program, int argc, char **argv)
{
    if (argc != 5 && argc != 6)
    {
        ft_putstr("Error: Wrong number of arguments\n");
        ft_putstr("Usage: ./philo number_of_philosophers time_to_die time_to_eat ");
        ft_putstr("time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
        return (1);
    }

    // Check if all arguments are valid numbers
    for (int i = 1; i < argc; i++)
    {
        if (!is_number(argv[i]))
        {
            ft_putstr("Error: Arguments must be positive numbers\n");
            return (1);
        }
    }

    // Parse arguments
    program->number_of_philosophers = ft_atoi(argv[1]);
    program->time_to_die = ft_atoi(argv[2]);
    program->time_to_eat = ft_atoi(argv[3]);
    program->time_to_sleep = ft_atoi(argv[4]);
    program->must_eat_count = -1;  // Default value if not specified
    
    if (argc == 6)
        program->must_eat_count = ft_atoi(argv[5]);

    // Validate arguments
    if (program->number_of_philosophers <= 0 || program->time_to_die <= 0 
        || program->time_to_eat <= 0 || program->time_to_sleep <= 0 
        || (argc == 6 && program->must_eat_count <= 0))
    {
        ft_putstr("Error: Invalid arguments (must be positive numbers)\n");
        return (1);
    }

    if (program->number_of_philosophers > 200)
    {
        ft_putstr("Error: Too many philosophers (maximum is 200)\n");
        return (1);
    }

    return (0);
}

int	main(int argc, char **argv)
{
    t_program	program;

    if (init_program_args(&program, argc, argv))
        return (1);
    if (init_program(&program))
    {
        ft_putstr("Error: Failed to initialize program\n");
        return (1);
    }
    clean_program(&program);
    return (0);
}