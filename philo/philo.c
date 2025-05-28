/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:41:42 by diogribe          #+#    #+#             */
/*   Updated: 2025/05/28 13:52:04 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_exit(t_vars *vars)
{
	if (vars->philos)
	{
		pthread_mutex_destroy(&vars->print_mutex);
		pthread_mutex_destroy(&vars->death_mutex);
		pthread_mutex_destroy(&vars->fork_manager_mutex);
	}
	free(vars->fork_available);
	free(vars->philos);
}

bool	init_vars(t_vars *vars, int ac, char **av)
{
	int	i;

	vars->n_philos = ft_atoi(av[1]);
	vars->t_die = ft_atoi(av[2]);
	vars->t_eat = ft_atoi(av[3]);
	vars->t_sleep = ft_atoi(av[4]);
	vars->dead = false;
	vars->start_time = get_time_ms();
	if (init_vars_utils(vars, ac, av) == false)
		return (false);
	i = 0;
	while (i < vars->n_philos)
		vars->fork_available[i++] = true;
	return (true);
}

void	init_philos(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->n_philos)
	{
		vars->philos[i].id = i + 1;
		vars->philos[i].meals_eaten = 0;
		vars->philos[i].vars = vars;
		vars->philos[i].left_fork_idx = i;
		vars->philos[i].right_fork_idx = (i + 1) % vars->n_philos;
		vars->philos[i].last_meal_time = get_time_ms();
		i++;
	}
}

void	philo_maker(t_vars *vars)
{
	pthread_t	monitor;
	int			i;

	i = 0;
	while (i < vars->n_philos)
	{
		if (pthread_create(&vars->philos[i].thread, NULL, &routine,
				&vars->philos[i]) != 0)
			exit_with_error(vars, "Error creating philosopher thread\n");
		i++;
	}
	if (pthread_create(&monitor, NULL, &monitor_death, vars) != 0)
		exit_with_error(vars, "Error creating monitor thread\n");
	pthread_join(monitor, NULL);
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

	memset(&vars, 0, sizeof(t_vars));
	if (ac < 5 || ac > 6)
		return (exit_with_error(&vars, "Error: Invalid number of arguments\n"));
	if (!init_vars(&vars, ac, av))
		return (exit_with_error(&vars, "Error initializing vars\n"));
	init_philos(&vars);
	philo_maker(&vars);
	clean_exit(&vars);
	return (0);
}
