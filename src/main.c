/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isoulima <soulimani.ilir@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:53:06 by isoulima          #+#    #+#             */
/*   Updated: 2026/05/20 16:13:52 by isoulima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup_rules(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		pthread_mutex_destroy(&rules->philos[i].meal_lock);
		i++;
	}
	pthread_mutex_destroy(&rules->stop_lock);
	pthread_mutex_destroy(&rules->print_lock);
	free(rules->forks);
	free(rules->philos);
}

static void	join_philos(t_rules *rules, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(rules->philos[i].thread, NULL);
		i++;
	}
}

static int	start_simulation(t_rules *rules)
{
	int	i;

	rules->start = get_time();
	i = 0;
	while (i < rules->nb)
	{
		rules->philos[i].last_meal = rules->start;
		if (pthread_create(&rules->philos[i].thread, NULL,
				philo_routine, &rules->philos[i]))
		{
			set_stop(rules);
			join_philos(rules, i);
			return (0);
		}
		i++;
	}
	monitor(rules);
	join_philos(rules, rules->nb);
	return (1);
}

int	main(int argc, char **argv)
{
	t_rules	rules;

	if (!parse_args(argc, argv, &rules))
	{
		print_error("Error: invalid arguments\n");
		return (print_error("Usage: ./philo number_of_philosophers "
				"time_to_die time_to_eat time_to_sleep "
				"[number_of_times_each_philosopher_must_eat]\n"));
	}
	if (!init_rules(&rules))
		return (print_error("Error: initialization failed\n"));
	if (!start_simulation(&rules))
	{
		cleanup_rules(&rules);
		return (print_error("Error: thread creation failed\n"));
	}
	cleanup_rules(&rules);
	return (0);
}
