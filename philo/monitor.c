#include "philosophers.h"

void	*monitor_routine(void *arg)
{
	t_program *prog;
	int i;

	prog = (t_program *)arg;
	while (!get_death_status(prog))
	{
        i = -1;
		while (++i < prog->number_of_philosophers)
		{
			if (get_time() - get_meal_time(&prog->philosophers[i]) > prog->time_to_die)
			{
                //printf("diffrence :%lu \n", get_time() - get_meal_time(&prog->philosophers[i]));
				set_death_status(prog->philosophers);
				print_status(prog->philosophers, "died");
				return (NULL);
			}
		}
	}
	return (NULL);
}