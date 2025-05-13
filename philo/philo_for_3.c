#include "philosophers.h"

void	take_forks_3(t_philosopher *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
}

int	eat_3(t_philosopher *philo)
{
	if (get_death_status(philo->program))
		return (1);
	pthread_mutex_lock(&philo->program->turn_mutex);
	while (philo->program->current_turn != philo->id)
	{
		pthread_mutex_unlock(&philo->program->turn_mutex);
		if (get_death_status(philo->program))
			return (1);
		pthread_mutex_lock(&philo->program->turn_mutex);
	}
	take_forks_3(philo);
	if (get_death_status(philo->program))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->program->turn_mutex);
		return (1);
	}
	print_status(philo, "is eating");
	set_meal_time(philo);
	ft_usleep(philo->program->time_to_eat, philo->program);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	if (!get_death_status(philo->program))
		philo->program->current_turn = (philo->program->current_turn + 1) % 3;
	pthread_mutex_unlock(&philo->program->turn_mutex);
	return (0);
}
