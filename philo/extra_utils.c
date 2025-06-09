/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:37:07 by diogribe          #+#    #+#             */
/*   Updated: 2025/06/06 16:16:52 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat_utils(t_philo *philo)
{
	philo->vars->fork_available[philo->left_fork_idx] = false;
	philo->vars->fork_available[philo->right_fork_idx] = false;
	pthread_mutex_unlock(&philo->vars->fork_manager_mutex);
	print_status(philo, "has taken a fork");
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->vars->death_mutex);
	philo->last_meal_time = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->vars->death_mutex);
	print_status(philo, "is eating");
	usleep(philo->vars->t_eat * 1000);
	pthread_mutex_lock(&philo->vars->fork_manager_mutex);
	philo->vars->fork_available[philo->left_fork_idx] = true;
	philo->vars->fork_available[philo->right_fork_idx] = true;
	pthread_mutex_unlock(&philo->vars->fork_manager_mutex);
}

bool	init_vars_utils(t_vars *vars, int ac, char **av)
{
	if (ac == 6)
		vars->meals_required = ft_atoi(av[5]);
	else
		vars->meals_required = -1;
	if (vars->n_philos <= 0 || vars->t_die <= 0 || vars->t_eat <= 0
		|| vars->t_sleep <= 0 || (ac == 6 && vars->meals_required <= 0))
		return (false);
	vars->philos = malloc(sizeof(t_philo) * vars->n_philos);
	vars->fork_available = malloc(sizeof(bool) * vars->n_philos);
	if (!vars->philos || !vars->fork_available)
	{
		free(vars->philos);
		free(vars->fork_available);
		return (false);
	}
	pthread_mutex_init(&vars->print_mutex, NULL);
	pthread_mutex_init(&vars->death_mutex, NULL);
	pthread_mutex_init(&vars->fork_manager_mutex, NULL);
	return (true);
}
