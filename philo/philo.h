/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:41:38 by diogribe          #+#    #+#             */
/*   Updated: 2025/05/28 13:50:56 by diogribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	int				left_fork_idx;
	int				right_fork_idx;
	struct s_vars	*vars;
	pthread_t		thread;
}				t_philo;

typedef struct s_vars
{
	int				n_philos;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				meals_required;
	long long		start_time;
	pthread_mutex_t	print_mutex;
	bool			*fork_available;
	pthread_mutex_t	fork_manager_mutex;
	t_philo			*philos;
	bool			dead;
	pthread_mutex_t	death_mutex;
}				t_vars;

/* Utils */

int			ft_atoi(const char *nptr);
long long	get_time_ms(void);
void		clean_exit(t_vars *vars);
int			exit_with_error(t_vars *vars, const char *msg);
size_t		ft_strlen(const char *s);

/* Routine */

void		*routine(void *arg);
void		print_status(t_philo *philo, char *status);

/* Death */

void		*monitor_death(void *arg);
long long	time_since_start(t_vars *vars);
bool		is_dead(t_vars *vars);

/* Extra Utils */

void		eat_utils(t_philo *philo);
bool		init_vars_utils(t_vars *vars, int ac, char **av);

#endif