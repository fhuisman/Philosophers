/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   routine.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/12 15:49:23 by fhuisman      #+#    #+#                 */
/*   Updated: 2023/07/17 13:42:56 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sleep(t_philo *p)
{
	pthread_mutex_lock(&(p->m_status));
	if (p->status == ALIVE)
		add_print(p->data, p, SLEEPING, get_time_ms(p));
	pthread_mutex_unlock(&(p->m_status));
	wait_for(p, p->data->time_to_sleep);
}

void	ft_take_forks(t_philo *p)
{
	if (p->philo_id % 2 == 1)
	{
		pthread_mutex_lock(p->l_fork);
		add_print(p->data, p, FORK, get_time_ms(p));
		pthread_mutex_lock(&(p->r_fork));
		add_print(p->data, p, FORK, get_time_ms(p));
	}
	else
	{
		pthread_mutex_lock(&(p->r_fork));
		add_print(p->data, p, FORK, get_time_ms(p));
		pthread_mutex_lock(p->l_fork);
		add_print(p->data, p, FORK, get_time_ms(p));
	}
}

void	ft_eat(t_philo *p)
{
	if (p->data->philo_count == 1)
	{
		pthread_mutex_lock(&(p->r_fork));
		add_print(p->data, p, FORK, get_time_ms(p));
		wait_for(p, p->data->time_to_die);
		pthread_mutex_unlock(&(p->r_fork));
		return ;
	}
	ft_take_forks(p);
	pthread_mutex_lock(&(p->m_status));
	p->time_last_meal = get_time_ms(p);
	p->meals_consumed++;
	if (p->status == ALIVE)
		add_print(p->data, p, EATING, get_time_ms(p));
	pthread_mutex_unlock(&(p->m_status));
	wait_for(p, p->data->time_to_eat);
	pthread_mutex_unlock(p->l_fork);
	pthread_mutex_unlock(&(p->r_fork));
}

void	ft_think(t_philo *p)
{
	add_print(p->data, p, THINKING, get_time_ms(p));
	usleep(500);
}

void	*routine(void *vargp)
{
	t_philo	*p;

	p = ((t_philo *) vargp);
	if (p->philo_id % 2 == 0)
		ft_think(p);
	pthread_mutex_lock(&(p->m_status));
	p->time_last_meal = 0;
	while (p->status == ALIVE)
	{
		pthread_mutex_unlock(&(p->m_status));
		ft_eat(p);
		ft_sleep(p);
		ft_think(p);
		pthread_mutex_lock(&(p->m_status));
	}
	pthread_mutex_unlock(&(p->m_status));
	return (NULL);
}
