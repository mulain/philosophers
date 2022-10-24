/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:27 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/24 21:39:40 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
• When a philosopher has finished eating, they put their forks back on the table and
start sleeping. Once awake, they start thinking again. The simulation stops when
a philosopher dies of starvation.

• Your(s) program(s) should take the following arguments:
number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]
◦ number_of_philosophers: The number of philosophers and also the number
of forks.
◦ time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die
milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.
◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
◦ number_of_times_each_philosopher_must_eat (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.
• Each philosopher has a number ranging from 1 to number_of_philosophers.
• Philosopher number 1 sits next to philosopher number number_of_philosophers.
Any other philosopher number N sits between philosopher number N - 1 and philosopher number N + 1.

About the logs of your program:
• Any state change of a philosopher must be formatted as follows:
◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died
Replace timestamp_in_ms with the current timestamp in milliseconds
and X with the philosopher number.
• A displayed state message should not be mixed up with another message.
• A message announcing a philosopher died should be displayed no more than 10 ms
after the actual death of the philosopher.
• Again, philosophers should avoid dying!

mandatory:
The specific rules for the mandatory part are:
• Each philosopher should be a thread.
• There is one fork between each pair of philosophers. Therefore, if there are several
philosophers, each philosopher has a fork on their left side and a fork on their right
side. If there is only one philosopher, there should be only one fork on the table.
• To prevent philosophers from duplicating forks, you should protect the forks state
with a mutex for each of them.
*/

/*
eat -> sleep -> think -> eat...
2 number of philosophers = number of forks (range: 1 to numb of phil)
3 time to die (ms)
4 time to eat (2 forks required, left and right of philo)
5 time to sleep (ms)
6 (number of times each philosopher must eat)
	optional, sim stops if all have eaten
	at least this number of times. if not specified,
	sim runs til one philo dies

old msg:
"Wrong number of arguments.\nFormat:\n./philosophers\
 n_philosophers time_to_die time_to_eat time_to_sleep (max_eat)"

 Allowed functions
memset,
printf, write,
malloc, free,
usleep, gettimeofday,
pthread_create, pthread_detach, pthread_join,
pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock
*/

void	*philosopher(void *arg)
{
	t_envl		*e;

	e = (t_envl *) arg;
	gettimeofday(&e->t, NULL);
	printf("tv_sec:%li\n", e->t.tv_sec);
	printf("tv_usec:%li\n", e->t.tv_usec);
	printf("ms:%li\n", e->t.tv_usec / 1000);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_envl		e;
	int			i;

	setup(&e, argc, argv);
	printf("n_philo:%i\n", e.n_philo);
	i = 0;
	while (i < e.n_philo)
	{
		if (pthread_create(e.threads + i, NULL, philosopher, &e))
			error_exit("Error: pthread_create");
		i++;
	}
	if (pthread_join(e.threads[0], NULL))
		error_exit("Error: pthread_join");
	printf("cray\n");
}

void	setup(t_envl *e, int argc, char **argv)
{
	int			i;

	check_input(argc, argv);
	parse_input(e, argc, argv);
	pthread_mutex_init(&e->print, NULL);
	e->forks = malloc(e->n_philo * sizeof(pthread_mutex_t));
	e->philostructs = malloc(e->n_philo * sizeof(t_philo));
	i = 0;
	while (i < e->n_philo)
	{
		pthread_mutex_init(e->forks + i, NULL);
		i++;
	}
	i = 0;
	while (i < e->n_philo)
	{
		set_philostruct(e, i);
		i++;
	}
	e->threads = malloc(e->n_philo * sizeof(pthread_t));
}

/*
input:			philo	n_phil	die		eat		sleep	max_eat
argv_index:		argv_0	argv_1	argv_2	argv_3	argv_4	argv_5
argc_value:		argc_1	argc_2	argc_3	argc_4	argc_5	argc_6
*/
void	check_input(int argc, char **argv)
{
	int		i;

	if (argc != 5 && argc != 6)
		error_exit("Wrong number of arguments.");
	if (!ispositiveint(argv[1]))
		error_exit("Only positive integers are valid for n_philosophers.");
	i = 2;
	while (i < argc)
	{
		if (!ispositiveintorzero(argv[i]))
			error_exit("Only integers >= 0 are valid for time / max_eat.");
		i++;
	}
}

void	parse_input(t_envl *e, int argc, char **argv)
{
	e->n_philo = ft_atoi(argv[1]);
	e->time_die = ft_atoi(argv[2]);
	e->time_eat = ft_atoi(argv[3]);
	e->time_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		e->max_eat = ft_atoi(argv[5]);
	else
		e->max_eat = -1;
}

void	set_philostruct(t_envl *e, int i)
{
	e->philostructs[i].name = i + 1;
	e->philostructs[i].time_die = e->time_die;
	e->philostructs[i].time_eat = e->time_eat;
	e->philostructs[i].time_sleep = e->time_sleep;
	e->philostructs[i].max_eat = e->max_eat;
	e->philostructs[i].print = e->print;
	e->philostructs[i].fork_n = e->forks[i];
	if (i != 0)
		e->philostructs[i].fork_n_minus1 = e->forks[i - 1];
	else
		e->philostructs[i].fork_n_minus1 = e->forks[e->n_philo];
}
