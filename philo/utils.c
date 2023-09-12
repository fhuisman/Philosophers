/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/12 15:20:56 by fhuisman      #+#    #+#                 */
/*   Updated: 2023/06/13 12:49:58 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *s)
{
	int	num;

	num = 0;
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (-1);
		if (num > (INT32_MAX - (*s - '0')) / 10)
			return (-1);
		num *= 10;
		num += *s - '0';
		s++;
	}
	if (num <= 0)
		return (-1);
	return (num);
}

void	wait_for(t_philo *p, int ms)
{
	int	start;
	int	time_passed;
	int	current;

	start = get_time_ms(p);
	time_passed = 0;
	pthread_mutex_lock(&(p->m_status));
	while (p->status == ALIVE && time_passed < ms)
	{
		pthread_mutex_unlock(&(p->m_status));
		usleep(500);
		current = get_time_ms(p);
		time_passed = get_time_ms(p);
		time_passed -= start;
		pthread_mutex_lock(&(p->m_status));
	}
	pthread_mutex_unlock(&(p->m_status));
}

int	get_time_ms(t_philo *philo)
{
	struct timeval	current;
	int				time;

	if (gettimeofday(&current, NULL) != 0)
		return (philo_error(ERR_GET_TIME, philo), -1);
	time = (current.tv_sec - philo->data->start.tv_sec) * 1000;
	time += (int)((current.tv_usec - philo->data->start.tv_usec) / 1000);
	return (time);
}

int	change_all_status(t_data *data, enum status status)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&(data->p[i].m_status));
		if (data->p[i].status != DEAD)
		{
			data->p[i].status = status;
		}
		pthread_mutex_unlock(&(data->p[i].m_status));
		i++;
	}
	return (-1);
}

void	free_print_list_philo_died(t_data *data)
{
	t_print	*temp;

	pthread_mutex_lock(&(data->m_print));
	while (data->print && data->print->msg != DIED)
	{
		temp = data->print;
		data->print = data->print->next;
		free(temp);
	}
	pthread_mutex_unlock(&(data->m_print));
	if (data->print && data->print->msg == DIED)
		print_message_from_print_list(data);
	pthread_mutex_lock(&(data->m_print));
	while (data->print)
	{
		temp = data->print;
		data->print = data->print->next;
		free(temp);
	}
	pthread_mutex_unlock(&(data->m_print));
}
