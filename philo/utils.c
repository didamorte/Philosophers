/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:58:01 by diogribe          #+#    #+#             */
/*   Updated: 2025/04/07 15:51:56 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	nbr;

	i = 1;
	nbr = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			i = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		nbr *= 10;
		nbr += *nptr - '0';
		nptr++;
	}
	return (nbr * i);
}

void	init_mutexes(t_vars *vars)
{
	int i = 0;
	while (i < vars->n_philos)
	{
		pthread_mutex_init(&vars->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&vars->print_mutex, NULL);
}

void	destroy_mutexes(t_vars *vars)
{
	int i = 0;
	while (i < vars->n_philos)
	{
		pthread_mutex_destroy(&vars->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&vars->print_mutex);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

int	exit_with_error(t_vars *vars, const char *msg)
{
	write(2, msg, strlen(msg));
	clean_exit(vars);
	return (1);
}

