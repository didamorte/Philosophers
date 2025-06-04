/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:13:20 by diogribe          #+#    #+#             */
/*   Updated: 2025/06/04 16:13:00 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->vars->print_mutex);
	if (!is_dead(philo->vars))
		printf("%lld %d %s\n", time_since_start(philo->vars), philo->id, status);
	pthread_mutex_unlock(&philo->vars->print_mutex);
}

void	eat(t_philo *philo)
{
	bool	has_eaten_this_turn;

	has_eaten_this_turn = false;
	while (!has_eaten_this_turn && !is_dead(philo->vars))
	{
		pthread_mutex_lock(&philo->vars->fork_manager_mutex);
		if (philo->vars->n_philos > 1
			&& philo->vars->fork_available[philo->left_fork_idx]
			&& philo->vars->fork_available[philo->right_fork_idx])
		{
			eat_utils(philo);
			has_eaten_this_turn = true;
		}
		else
		{
			pthread_mutex_unlock(&philo->vars->fork_manager_mutex);
			if (philo->vars->n_philos > 1)
				usleep(250);
		}
	}
}

void	sleep_philo(t_philo *philo)
{
	print_status(philo, "is sleeping");
	usleep(philo->vars->t_sleep * 1000);
}

void	think(t_philo *philo)
{
	print_status(philo, "is thinking");
	usleep(5000);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(philo->vars->t_eat * 5);
	while (!is_dead(philo->vars))
	{
		if (philo->vars->meals_required != -1
			&& philo->meals_eaten >= philo->vars->meals_required)
			break ;
		eat(philo);
		if (is_dead(philo->vars))
			break ;
		sleep_philo(philo);
		if (is_dead(philo->vars))
			break ;
		think(philo);
	}
	return (NULL);
}
