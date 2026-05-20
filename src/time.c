/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isoulima <soulimani.ilir@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 15:37:21 by isoulima          #+#    #+#             */
/*   Updated: 2026/05/20 16:31:09 by isoulima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	get_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000L + tv.tv_usec);
}

long	get_time(void)
{
	return (get_time_us() / 1000);
}

void	ft_usleep(long ms, t_rules *rules)
{
	long	end;

	end = get_time_us() + ms * 1000;
	while (get_time_us() < end)
	{
		if (is_stopped(rules))
			return ;
		usleep(500);
	}
}

void	print_status(t_philo *philo, char *msg)
{
	long	timestamp;

	pthread_mutex_lock(&philo->rules->print_lock);
	if (!is_stopped(philo->rules))
	{
		timestamp = get_time() - philo->rules->start;
		printf("%ld %d %s\n", timestamp, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->rules->print_lock);
}
