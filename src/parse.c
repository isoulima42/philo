/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isoulima <soulimani.ilir@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 11:28:44 by isoulima          #+#    #+#             */
/*   Updated: 2026/05/20 16:19:14 by isoulima         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	print_error(char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

static int	parse_number(char *s, long *out)
{
	long	n;
	int		i;

	i = 0;
	n = 0;
	if (s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		n = n * 10 + (s[i] - '0');
		if (n > INT_MAX_VAL)
			return (0);
		i++;
	}
	*out = n;
	return (1);
}

int	parse_args(int argc, char **argv, t_rules *rules)
{
	long	values[5];
	int		i;

	if (argc != 5 && argc != 6)
		return (0);
	values[4] = -1;
	i = 0;
	while (i < argc - 1)
	{
		if (!parse_number(argv[i + 1], &values[i]) || values[i] <= 0)
			return (0);
		i++;
	}
	rules->nb = (int)values[0];
	rules->t_die = values[1];
	rules->t_eat = values[2];
	rules->t_sleep = values[3];
	rules->must_eat = (int)values[4];
	return (1);
}
