/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diogribe <diogribe@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:41:38 by diogribe          #+#    #+#             */
/*   Updated: 2025/04/07 16:52:23 by diogribe         ###   ########.fr       */
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
#include <stdbool.h>

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
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
	pthread_mutex_t	*forks;
	t_philo			*philos;
	bool			dead;
	pthread_mutex_t	death_mutex;
}				t_vars;

/* Utils */

int			ft_atoi(const char *nptr);
long long	get_time_ms(void);
void		clean_exit(t_vars *vars);
int			exit_with_error(t_vars *vars, const char *msg);

/* Routine */

void		*routine(void *arg);

/* Death */

void		*monitor_death(void *arg);
long long	time_since_start(t_vars *vars);
bool	is_dead(t_vars *vars);

#endif