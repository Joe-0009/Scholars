#include "philosophers.h"

static int	check_meals_count(t_philosopher *philo)
{
	int	enough_meals;

	enough_meals = 0;
	if (philo->program->must_eat_count != -1)
	{
		pthread_mutex_lock(&philo->meal_mutex);
		if (philo->number_of_meals >= philo->program->must_eat_count)
			enough_meals = 1;
		pthread_mutex_unlock(&philo->meal_mutex);
	}
	return (enough_meals);
}

static int	check_if_all_ate(t_program *prog)
{
	int	i;
	int	all_ate;

	if (prog->must_eat_count == -1)
		return (0);
	i = -1;
	all_ate = 1;
	while (++i < prog->number_of_philosophers)
	{
		if (!check_meals_count(&prog->philosophers[i]))
		{
			all_ate = 0;
			break ;
		}
	}
	return (all_ate);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2 == 0)
		usleep(10 * 1000);
	while (!get_death_status(philo->program))
	{
		if (eat(philo))
			break ;
		if (sleep_think_actions(philo))
			break ;
	}
	return (NULL);
}

void	*philosopher_routine_3(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (!get_death_status(philo->program))
	{
		if (eat_3(philo))
			break ;
		if (sleep_think_actions(philo))
			break ;
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_program	*prog;
	int			i;
	long		time_since_meal;
	
	prog = (t_program *)arg;
	while (!get_death_status(prog))
	{
		i = -1;
		while (++i < prog->number_of_philosophers)
		{
			time_since_meal = get_time() - get_meal_time(&prog->philosophers[i]);
			if (time_since_meal >= prog->time_to_die)
			{
				set_death_status(prog->philosophers);
				print_status(&prog->philosophers[i], "died");
				return (NULL);
			}
		}
		if (prog->must_eat_count != -1 && check_if_all_ate(prog))
		{
			set_death_status(prog->philosophers);
			return (NULL);
		}
		usleep(2 * 1000);
	}
	return (NULL);
}
