
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>


typedef struct s_philosophers
{
	int id;
	int times_eaten;
	int last_meal_time;
	pthread_mutex_t *left_fork;
	pthread_mutex_t *right_fork;
	struct s_program *program;
	
}t_philosophers;

typedef struct s_program
{
    int             number_of_philosophers;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             must_eat_count;
	long long start_time;
	int				some_one_died;
	pthread_mutex_t *forks;
	pthread_t	*threads;
	t_philosophers *philosophers;
} t_program;




int init_program(t_program *program);
void clean_program(t_program *program);

#endif