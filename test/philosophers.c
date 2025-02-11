
#include "philosophers.h"

long long	get_current_time(void)
{
    struct timeval	time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_status(t_philosopher *philo, char *status)
{
    long long	current_time;

    pthread_mutex_lock(philo->print_mutex);
    if (!philo->program->someone_died)
    {
        current_time = get_current_time() - philo->program->start_time;
        printf("%lld %d %s\n", current_time, philo->id + 1, status);
    }
    pthread_mutex_unlock(philo->print_mutex);
}

static void	eat(t_philosopher *philo)
{
    pthread_mutex_lock(philo->left_fork);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(philo->right_fork);
    print_status(philo, "has taken a fork");
    
    print_status(philo, "is eating");
    philo->last_meal = get_current_time();
    usleep(philo->program->time_to_eat * 1000);
    philo->times_eaten++;
    
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

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

void	*philosopher_routine(void *arg)
{
    t_philosopher	*philo;

    philo = (t_philosopher *)arg;
    if (philo->id % 2)
        usleep(1000);
    while (!philo->program->someone_died)
    {
        eat(philo);
        if (philo->program->must_eat_count != -1 && 
            philo->times_eaten >= philo->program->must_eat_count)
            break ;
        print_status(philo, "is sleeping");
        usleep(philo->program->time_to_sleep * 1000);
        print_status(philo, "is thinking");
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

int	init_program(t_program *program)
{
    int	i;

    program->someone_died = 0;
    program->forks = malloc(sizeof(pthread_mutex_t) * program->number_of_philosophers);
    program->threads = malloc(sizeof(pthread_t) * program->number_of_philosophers);
    program->philosophers = malloc(sizeof(t_philosopher) * program->number_of_philosophers);
    if (!program->forks || !program->threads || !program->philosophers)
        return (1);
    if (pthread_mutex_init(&program->print_mutex, NULL))
        return (1);
    i = 0;
    while (i < program->number_of_philosophers)
    {
        if (pthread_mutex_init(&program->forks[i], NULL))
            return (1);
        program->philosophers[i].id = i;
        program->philosophers[i].times_eaten = 0;
        program->philosophers[i].left_fork = &program->forks[i];
        program->philosophers[i].right_fork = &program->forks[(i + 1) % 
            program->number_of_philosophers];
        program->philosophers[i].print_mutex = &program->print_mutex;
        program->philosophers[i].program = program;
        i++;
    }
    return (create_threads(program));
}

void	clean_program(t_program *program)
{
    int	i;

    i = 0;
    while (i < program->number_of_philosophers)
    {
        pthread_join(program->threads[i], NULL);
        pthread_mutex_destroy(&program->forks[i]);
        i++;
    }
    pthread_mutex_destroy(&program->print_mutex);
    free(program->forks);
    free(program->threads);
    free(program->philosophers);
}