/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/11 13:57:02 by fhuisman      #+#    #+#                 */
/*   Updated: 2023/06/16 17:24:48 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	write_error(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	write(2, "Error: ", 7);
	write(2, str, len);
	write(2, "\n", 1);
	return (-1);
}

void	philo_error(char *message, t_philo *philo)
{
	pthread_mutex_lock(&(philo->m_status));
	philo->status = ERROR;
	pthread_mutex_unlock(&(philo->m_status));
	write_error(message);
}

void	destr_mut(t_data *data, int i)
{
	while (--i >= 0)
	{
		pthread_mutex_destroy(&(data->p[i].r_fork));
		pthread_mutex_destroy(&(data->p[i].m_status));
	}
}

void	detach_threads(t_data *data, int from, int to)
{
	while (from < to)
		pthread_detach(data->p[from++].p_thread);
}

void	free_all(t_data *data)
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
	destr_mut(data, data->philo_count);
	pthread_mutex_destroy(&(data->m_print));
	free(data->p);
}
