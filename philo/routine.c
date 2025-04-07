/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 14:13:20 by diogribe          #+#    #+#             */
/*   Updated: 2025/04/07 16:52:04 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->vars->print_mutex);
	printf("%lld %d %s\n", time_since_start(philo->vars), philo->id, status);
	pthread_mutex_unlock(&philo->vars->print_mutex);
}

void	eat(t_philo *philo)
{
	printf("Número de garfos: %d\n", philo->vars->n_philos);
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");

	// Verifique se o garfo da direita está disponível
	if (pthread_mutex_trylock(philo->right_fork) == 0)
	{
		print_status(philo, "has taken a fork");
		print_status(philo, "is eating");
		philo->last_meal_time = get_time_ms();
		usleep(philo->vars->t_eat * 1000);

		// Liberar os garfos após a refeição
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		// Liberar o garfo da esquerda e tentar novamente
		pthread_mutex_unlock(philo->left_fork);
		usleep(1000); // Pausa pequena para evitar competição intensa
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
	usleep(1000);
}

void	*routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;

	while (!is_dead(philo->vars))
	{
		eat(philo);
		sleep_philo(philo);
		think(philo);
	}
	return (NULL);
}
