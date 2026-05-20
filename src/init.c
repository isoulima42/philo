/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isoulima <soulimani.ilir@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 09:51:33 by isoulima          #+#    #+#             */
/*   Updated: 2026/05/20 16:24:47 by isoulima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	destroy_forks(t_rules *rules, int count)
{
	while (--count >= 0)
		pthread_mutex_destroy(&rules->forks[count]);
	free(rules->forks);
}

static int	init_forks(t_rules *rules)
{
	int	i;

	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->nb);
	if (!rules->forks)
		return (0);
	i = 0;
	while (i < rules->nb)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL))
		{
			destroy_forks(rules, i);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	set_forks(t_rules *rules, t_philo *philo, int i)
{
	int	left;
	int	right;

	left = i;
	right = (i + 1) % rules->nb;
	if (left < right)
	{
		philo->first_fork = &rules->forks[left];
		philo->second_fork = &rules->forks[right];
	}
	else
	{
		philo->first_fork = &rules->forks[right];
		philo->second_fork = &rules->forks[left];
	}
}

static int	init_philos(t_rules *rules)
{
	int	i;

	rules->philos = malloc(sizeof(t_philo) * rules->nb);
	if (!rules->philos)
		return (0);
	memset(rules->philos, 0, sizeof(t_philo) * rules->nb);
	i = 0;
	while (i < rules->nb)
	{
		rules->philos[i].id = i + 1;
		rules->philos[i].rules = rules;
		set_forks(rules, &rules->philos[i], i);
		if (pthread_mutex_init(&rules->philos[i].meal_lock, NULL))
		{
			while (--i >= 0)
				pthread_mutex_destroy(&rules->philos[i].meal_lock);
			free(rules->philos);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_rules(t_rules *rules)
{
	rules->stop = 0;
	if (pthread_mutex_init(&rules->stop_lock, NULL))
		return (0);
	if (pthread_mutex_init(&rules->print_lock, NULL))
	{
		pthread_mutex_destroy(&rules->stop_lock);
		return (0);
	}
	if (!init_forks(rules))
	{
		pthread_mutex_destroy(&rules->stop_lock);
		pthread_mutex_destroy(&rules->print_lock);
		return (0);
	}
	if (!init_philos(rules))
	{
		destroy_forks(rules, rules->nb);
		pthread_mutex_destroy(&rules->stop_lock);
		pthread_mutex_destroy(&rules->print_lock);
		return (0);
	}
	return (1);
}
