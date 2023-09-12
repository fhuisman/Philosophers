/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/12 15:18:39 by fhuisman      #+#    #+#                 */
/*   Updated: 2023/05/12 14:55:50 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
		return (write_error(ERR_USAGE));
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->meal_count = ft_atoi(argv[5]);
	else
		data->meal_count = -2;
	data->print = NULL;
	if (data->philo_count == -1 || data->time_to_die == -1 || data->time_to_eat
		== -1 || data->time_to_sleep == -1)
		return (write_error(ERR_USAGE));
	if (argc == 6)
		if (data->meal_count == -1)
			return (write_error(ERR_USAGE));
	if (gettimeofday(&(data->start), NULL))
		return (write_error(ERR_GET_TIME));
	return (0);
}

int	init_philos(t_data *data)
{
	int		i;

	data->p = NULL;
	data->p = (t_philo *) malloc((data->philo_count) * sizeof(t_philo));
	if (!data->p)
		return (write_error(ERR_MALLOC));
	i = 0;
	while (i < data->philo_count)
	{
		data->p[i].philo_id = i + 1;
		data->p[i].meals_consumed = 0;
		data->p[i].data = data;
		data->p[i].status = ALIVE;
		data->p[((i + 1) % data->philo_count)].l_fork = &(data->p[i].r_fork);
		i++;
	}
	return (0);
}

int	init_mutex(t_data *data)
{
	int				i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_mutex_init(&(data->p[i].r_fork), NULL))
			return (destr_mut(data, i), write_error(ERR_MUTEX_INIT));
		if (pthread_mutex_init(&(data->p[i].m_status), NULL))
		{
			pthread_mutex_destroy(&(data->p[i].r_fork));
			return (destr_mut(data, i), write_error(ERR_MUTEX_INIT));
		}
		i++;
	}
	if (pthread_mutex_init(&(data->m_print), NULL))
		return (destr_mut(data, i), write_error(ERR_MUTEX_INIT));
	return (0);
}

int	init(t_data *data, int argc, char *argv[])
{
	if (init_data(data, argc, argv))
		return (-1);
	if (init_philos(data))
		return (-1);
	if (init_mutex(data))
		return (free(data->p), -1);
	return (0);
}

int	init_routine(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_count)
		if (pthread_create(&(data->p[i].p_thread), NULL,
				routine, &(data->p[i])))
			return (detach_threads(data, 0, i),
				write_error(ERR_CREATE_THREAD));
	if (pthread_create(&data->print_thread, NULL, printer, data))
		return (detach_threads(data, 0, i),
			write_error(ERR_CREATE_THREAD));
	i = -1;
	while (++i < data->philo_count)
		if (pthread_join(data->p[i].p_thread, NULL))
			return (detach_threads(data, i, data->philo_count),
				write_error(ERR_JOIN_THREAD));
	if (pthread_join(data->print_thread, NULL))
	{
		pthread_detach(data->print_thread);
		return (write_error(ERR_JOIN_THREAD));
	}
	return (0);
}
