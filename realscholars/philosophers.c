#include "philosophers.h"

long long get_current_time(void)
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int init_program(t_program *program)
{
    int i;

    program->some_one_died = 0;
    program->forks = malloc(program->number_of_philosophers * sizeof(pthread_mutex_t));
    program->philosophers = malloc(program->number_of_philosophers * sizeof(t_philosophers));
    program->threads = malloc(program->number_of_philosophers * sizeof(pthread_t));
    if (!program->forks || !program->philosophers || !program->threads)
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