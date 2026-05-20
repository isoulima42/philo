/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isoulima <soulimani.ilir@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 10:42:19 by isoulima          #+#    #+#             */
/*   Updated: 2026/05/20 16:08:37 by isoulima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define INT_MAX_VAL 2147483647
# define MAX_THINK 600

typedef struct s_rules	t_rules;

typedef struct s_philo
{
	int				id;
	int				meals;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;
	t_rules			*rules;
}	t_philo;

struct s_rules
{
	int				nb;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	int				must_eat;
	long			start;
	int				stop;
	pthread_mutex_t	stop_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
};

int		parse_args(int argc, char **argv, t_rules *rules);
int		print_error(char *msg);

int		init_rules(t_rules *rules);
void	cleanup_rules(t_rules *rules);

long	get_time(void);
void	ft_usleep(long ms, t_rules *rules);
void	print_status(t_philo *philo, char *msg);

int		is_stopped(t_rules *rules);
void	set_stop(t_rules *rules);
void	monitor(t_rules *rules);

void	*philo_routine(void *arg);

#endif
