#include "philosophers.h"

static void	*monitor_routine(void *arg)
{
    t_program		*program;
    t_philosopher	*philo;
    int				i;

    program = (t_program *)arg;
    while (!program->someone_died)
    {
        i = 0;
        while (i < program->number_of_philosophers)
        {
            philo = &program->philosophers[i];
            if (get_current_time() - philo->last_meal > program->time_to_die)
            {
                pthread_mutex_lock(&program->print_mutex);
                printf("%lld %d died\n", 
                    get_current_time() - program->start_time, philo->id + 1);
                program->someone_died = 1;
                pthread_mutex_unlock(&program->print_mutex);
                return (NULL);
            }
            if (program->must_eat_count != -1 && 
                philo->times_eaten >= program->must_eat_count)
                i++;
            else
                i = 0;
            usleep(100);
        }
        if (i == program->number_of_philosophers)
            break ;
    }
    return (NULL);
}

static int	create_threads(t_program *program)
{
    pthread_t	monitor;
    int			i;

    program->start_time = get_current_time();
    i = 0;
    while (i < program->number_of_philosophers)
    {
        program->philosophers[i].last_meal = get_current_time();
        if (pthread_create(&program->threads[i], NULL, 
            philosopher_routine, &program->philosophers[i]))
            return (1);
        i++;
    }
    if (pthread_create(&monitor, NULL, monitor_routine, program))
        return (1);
    pthread_detach(monitor);
    return (0);
}

int init_program(t_program *program)
{
    int i;

    program->someone_died = 0;
    program->forks = malloc(program->number_of_philosophers * sizeof(pthread_mutex_t));
    program->philosophers = malloc(program->number_of_philosophers * sizeof(t_philosopher));
    program->threads = malloc(program->number_of_philosophers * sizeof(pthread_t));
    if (!program->forks || !program->philosophers || !program->threads)
        return (1);
    if (pthread_mutex_init(&program->print_mutex, NULL))
        return (1);
    i = -1;
    while (++i < program->number_of_philosophers)
    {
        if (pthread_mutex_init(&program->forks[i], NULL))
            return (1);
        program->philosophers[i].id = i;
        program->philosophers[i].times_eaten = 0;
        program->philosophers[i].left_fork = &program->forks[i];
        program->philosophers[i].right_fork = &program->forks[(i + 1) % program->number_of_philosophers];
        program->philosophers[i].program = program;
    }
    return (create_threads(program));
}

void clean_program(t_program *program)
{
    int i;

    i = -1;
    while (++i < program->number_of_philosophers)
    {
        pthread_join(program->threads[i], NULL);
        pthread_mutex_destroy(&program->forks[i]);
    }
    free(program->forks);
    free(program->threads);
    free(program->philosophers);
}
