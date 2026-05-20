/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isoulima <soulimani.ilir@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 13:46:12 by isoulima          #+#    #+#             */
/*   Updated: 2026/05/20 16:44:53 by isoulima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_stopped(t_rules *rules)
{
	int	stop;

	pthread_mutex_lock(&rules->stop_lock);
	stop = rules->stop;
	pthread_mutex_unlock(&rules->stop_lock);
	return (stop);
}

void	set_stop(t_rules *rules)
{
	pthread_mutex_lock(&rules->stop_lock);
	rules->stop = 1;
	pthread_mutex_unlock(&rules->stop_lock);
}

static int	philo_died(t_rules *rules, t_philo *philo)
{
	long	last_meal;
	long	now;

	pthread_mutex_lock(&philo->meal_lock);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_lock);
	now = get_time();
	if (now - last_meal <= rules->t_die)
		return (0);
	pthread_mutex_lock(&rules->print_lock);
	set_stop(rules);
	printf("%ld %d died\n", now - rules->start, philo->id);
	pthread_mutex_unlock(&rules->print_lock);
	return (1);
}

static int	all_full(t_rules *rules)
{
	int	i;
	int	full;

	if (rules->must_eat < 0)
		return (0);
	full = 1;
	i = 0;
	while (i < rules->nb && full)
	{
		pthread_mutex_lock(&rules->philos[i].meal_lock);
		if (rules->philos[i].meals < rules->must_eat)
			full = 0;
		pthread_mutex_unlock(&rules->philos[i].meal_lock);
		i++;
	}
	if (full)
		set_stop(rules);
	return (full);
}

void	monitor(t_rules *rules)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < rules->nb)
		{
			if (philo_died(rules, &rules->philos[i]))
				return ;
			i++;
		}
		if (all_full(rules))
			return ;
		usleep(500);
	}
}
