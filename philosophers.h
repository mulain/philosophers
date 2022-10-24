/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:59 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/24 21:30:47 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H

# define PHILOSOPHERS_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct philostruct
{
	int					name;
	int					time_die;
	int					time_eat;
	int					time_sleep;
	int					max_eat;
	int					starttime;
	pthread_mutex_t		print;
	pthread_mutex_t		fork_n;
	pthread_mutex_t		fork_n_minus1;
}	t_philo;

typedef struct envelope
{
	int					n_philo;
	int					time_die;
	int					time_eat;
	int					time_sleep;
	int					max_eat;
	int					starttime;
	pthread_mutex_t		print;
	pthread_t			*threads;
	pthread_mutex_t		*forks;
	t_philo				*philostructs;
	struct timeval		t;
}	t_envl;

//0_main.c
int		main(int argc, char **argv);
void	setup(t_envl *e, int argc, char **argv);
void	check_input(int argc, char **argv);
void	parse_input(t_envl *e, int argc, char **argv);
void	set_philostruct(t_envl *e, int i);

//6_utils.c
int		ispositiveint(char *input);
int		ispositiveintorzero(char *input);
int		ft_atoi(const char *nptr);

//8_errors.c
void	error_exit(char *msg);

#endif