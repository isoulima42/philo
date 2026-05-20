/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isoulima <soulimani.ilir@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/08 10:14:58 by isoulima          #+#    #+#             */
/*   Updated: 2026/05/20 16:38:26 by isoulima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*lonely_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	print_status(philo, "has taken a fork");
	while (!is_stopped(philo->rules))
		usleep(500);
	pthread_mutex_unlock(philo->first_fork);
	return (NULL);
}

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(philo->second_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->meal_lock);
	print_status(philo, "is eating");
	ft_usleep(philo->rules->t_eat, philo->rules);
	pthread_mutex_lock(&philo->meal_lock);
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_lock);
	pthread_mutex_unlock(philo->second_fork);
	pthread_mutex_unlock(philo->first_fork);
}

static void	think(t_philo *philo)
{
	long	time_left;
	long	think_time;

	pthread_mutex_lock(&philo->meal_lock);
	time_left = philo->rules->t_die - (get_time() - philo->last_meal);
	pthread_mutex_unlock(&philo->meal_lock);
	think_time = (time_left - philo->rules->t_eat) / 2;
	if (think_time > MAX_THINK)
		think_time = MAX_THINK;
	print_status(philo, "is thinking");
	if (think_time > 0)
		ft_usleep(think_time, philo->rules);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->rules->nb == 1)
		return (lonely_philo(philo));
	if (philo->id % 2 == 0)
		think(philo);
	while (!is_stopped(philo->rules))
	{
		eat(philo);
		print_status(philo, "is sleeping");
		ft_usleep(philo->rules->t_sleep, philo->rules);
		think(philo);
	}
	return (NULL);
}
