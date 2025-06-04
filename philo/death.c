/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:20:18 by diogribe          #+#    #+#             */
/*   Updated: 2025/06/04 16:12:19 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_dead(t_vars *vars)
{
	bool	value;

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

static int	detect_and_set_first_dying_philo(t_vars *vars)
{
	int			i;
	long long	current_time;
	long long	philo_last_meal;

	current_time = get_time_ms();
	i = 0;
	while (i < vars->n_philos)
	{
		pthread_mutex_lock(&vars->death_mutex);
		if (vars->dead)
		{
			pthread_mutex_unlock(&vars->death_mutex);
			return (0);
		}
		philo_last_meal = vars->philos[i].last_meal_time;
		if (current_time - philo_last_meal > vars->t_die)
		{
			vars->dead = true;
			pthread_mutex_unlock(&vars->death_mutex);
			return (vars->philos[i].id);
		}
		pthread_mutex_unlock(&vars->death_mutex);
		i++;
	}
	return (-1);
}

static bool	check_meal_completion_status(t_vars *vars)
{
	int		k;
	int		eats_count;

	k = 0;
	while (k < vars->n_philos)
	{
		pthread_mutex_lock(&vars->death_mutex);
		eats_count = vars->philos[k].meals_eaten;
		pthread_mutex_unlock(&vars->death_mutex);
		if (eats_count < vars->meals_required)
			return (false);
		k++;
	}
	return (true);
}

void	*monitor_death(void *arg)
{
	t_vars	*vars;
	int		dead_philo_id;

	vars = (t_vars *)arg;
	if (vars->n_philos == 1)
		usleep(vars->t_die * 500 + 100);
	else if (vars->n_philos > 0)
		usleep(vars->t_die * 500);
	while (!is_dead(vars))
	{
		dead_philo_id = detect_and_set_first_dying_philo(vars);
		if (dead_philo_id > 0)
		{
			pthread_mutex_lock(&vars->print_mutex);
			printf("%lld %d died\n", time_since_start(vars), dead_philo_id);
			pthread_mutex_unlock(&vars->print_mutex);
		}
		if (!is_dead(vars) && vars->meals_required > 0
			&& check_meal_completion_status(vars))
			set_dead(vars);
		if (!is_dead(vars))
			usleep(500);
	}
	return (NULL);
}
