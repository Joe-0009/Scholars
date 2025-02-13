#include "philosophers.h"

long long get_current_time(void)
{
    struct timeval time;

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

