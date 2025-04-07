/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:41:42 by diogribe          #+#    #+#             */
/*   Updated: 2025/04/07 16:53:14 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_exit(t_vars *vars)
{
	for (int i = 0; i < vars->n_philos; i++)
		pthread_mutex_destroy(&vars->forks[i]);

	pthread_mutex_destroy(&vars->print_mutex);
	pthread_mutex_destroy(&vars->death_mutex);

	free(vars->forks);
	free(vars->philos);
}

bool	init_vars(t_vars *vars, int ac, char **av)
{
	if (ac < 5)
		return (false);

	vars->n_philos = ft_atoi(av[1]);
	vars->t_die = ft_atoi(av[2]);
	vars->t_eat = ft_atoi(av[3]);
	vars->t_sleep = ft_atoi(av[4]);
	vars->dead = false;
	vars->start_time = get_time_ms();

	// Alocar arrays
	vars->forks = malloc(sizeof(pthread_mutex_t) * vars->n_philos);
	vars->philos = malloc(sizeof(t_philo) * vars->n_philos);
	if (!vars->forks || !vars->philos)
		return (false);

	// Inicializar mutexes
	pthread_mutex_init(&vars->print_mutex, NULL);
	pthread_mutex_init(&vars->death_mutex, NULL);
	for (int i = 0; i < vars->n_philos; i++)
		pthread_mutex_init(&vars->forks[i], NULL);

	return (true);
}

void	init_philos(t_vars *vars)
{
	int i = 0;
	while (i < vars->n_philos)
	{
		vars->philos[i].id = i + 1;
		vars->philos[i].vars = vars;
		vars->philos[i].left_fork = &vars->forks[i];
		vars->philos[i].right_fork = &vars->forks[(i + 1) % vars->n_philos];
		vars->philos[i].last_meal_time = get_time_ms();
		i++;
	}
}

void	philo_maker(t_vars *vars)
{
	int	i;
	pthread_t	monitor;

	i = 0;
	while (i < vars->n_philos)
	{
		vars->philos[i].id = i + 1;
		vars->philos[i].last_meal_time = get_time_ms();
		vars->philos[i].left_fork = &vars->forks[i];
		vars->philos[i].right_fork = &vars->forks[(i + 1) % vars->n_philos];
		vars->philos[i].vars = vars;

		if (pthread_create(&vars->philos[i].thread, NULL, &routine, &vars->philos[i]) != 0)
			printf("Error creating thread\n");
		i++;
	}
	// 🔍 Criação do monitor de morte
	if (pthread_create(&monitor, NULL, &monitor_death, vars) != 0)
		printf("Error creating monitor thread\n");
	// Espera o monitor
	pthread_join(monitor, NULL);
	// 🧵 Espera os filósofos
	i = 0;
	while (i < vars->n_philos)
	{
		pthread_join(vars->philos[i].thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_vars	vars;

	if (!init_vars(&vars, ac, av))
		return (exit_with_error(&vars, "Error initializing vars\n"));

	init_philos(&vars);
	philo_maker(&vars);
	clean_exit(&vars);
	return (0);
}
