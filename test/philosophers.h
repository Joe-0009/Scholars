
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_philosopher
{
    int             id;
    int             times_eaten;
    long long       last_meal;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    pthread_mutex_t *print_mutex;
    struct s_program *program;
} t_philosopher;

typedef struct s_program
{
    int             number_of_philosophers;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             must_eat_count;
    long long       start_time;
    int             someone_died;
    pthread_mutex_t *forks;
    pthread_mutex_t print_mutex;
    pthread_t       *threads;
    t_philosopher   *philosophers;
} t_program;

// Function prototypes
int     init_program_args(t_program *program, int argc, char **argv);
int     init_program(t_program *program);
void    *philosopher_routine(void *arg);
long long get_current_time(void);
void    print_status(t_philosopher *philo, char *status);
void    clean_program(t_program *program);

#endif