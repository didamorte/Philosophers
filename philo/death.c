/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:20:18 by diogribe          #+#    #+#             */
/*   Updated: 2025/04/07 16:54:31 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	time_since_start(t_vars *vars)
{
	return (get_time_ms() - vars->start_time);
}

bool	is_dead(t_vars *vars)
{
	bool value;

	pthread_mutex_lock(&vars->death_mutex);
	value = vars->dead;
	pthread_mutex_unlock(&vars->death_mutex);
	return (value);
}

void	set_dead(t_vars *vars)
{
	pthread_mutex_lock(&vars->death_mutex);
	vars->dead = true;
	pthread_mutex_unlock(&vars->death_mutex);
}

void	*monitor_death(void *arg)
{
	t_vars *vars = (t_vars *)arg;
	int i;

	while (!is_dead(vars))
	{
		i = 0;
		while (i < vars->n_philos && !is_dead(vars))
		{
			long long now = get_time_ms();
			if (now - vars->philos[i].last_meal_time > vars->t_die)
			{
				pthread_mutex_lock(&vars->print_mutex);
				printf("%lld %d died\n", time_since_start(vars), vars->philos[i].id);
				pthread_mutex_unlock(&vars->print_mutex);
				set_dead(vars);
				break;
			}
			i++;
		}
		usleep(1000);
	}
	i = -1;
	while (++i < vars->n_philos)
		pthread_cancel(vars->philos[i].thread);  // Interromper as threads de filósofos
	return (NULL);
}
