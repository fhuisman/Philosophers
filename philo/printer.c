/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   printer.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/02 14:49:37 by fhuisman      #+#    #+#                 */
/*   Updated: 2023/06/16 15:43:31 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	add_print(t_data *data, t_philo *philo, t_msg msg, int time_ms)
{
	t_print	*new;
	t_print	*temp;

	new = NULL;
	new = malloc(sizeof(t_print));
	if (!new)
		return (philo_error(ERR_MALLOC, philo));
	new->msg = msg;
	new->philo_id = philo->philo_id;
	new->time_ms = time_ms;
	new->next = NULL;
	pthread_mutex_lock(&(data->m_print));
	if (data->print == NULL)
		data->print = new;
	else
	{
		temp = data->print;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
	pthread_mutex_unlock(&(data->m_print));
}

void	print_message(t_data *data, t_print print)
{
	if (print.msg == EATING)
		printf("%d %d %s\n", print.time_ms, print.philo_id, MSG_EATING);
	if (print.msg == SLEEPING)
		printf("%d %d %s\n", print.time_ms, print.philo_id, MSG_SLEEPING);
	if (print.msg == THINKING)
		printf("%d %d %s\n", print.time_ms, print.philo_id, MSG_THINKING);
	if (print.msg == DIED)
		printf("%d %d %s\n", get_time_ms(&(data->p[print.philo_id - 1])),
			print.philo_id, MSG_DIED);
	if (print.msg == FORK)
		printf("%d %d %s\n", print.time_ms, print.philo_id, MSG_FORK);
}

int	print_message_from_print_list(t_data *data)
{
	t_print	print;
	t_print	*temp;

	pthread_mutex_lock(&(data->m_print));
	if (data->print)
	{
		temp = data->print;
		data->print = data->print->next;
		pthread_mutex_unlock(&(data->m_print));
		print.philo_id = temp->philo_id;
		print.msg = temp->msg;
		print.time_ms = temp->time_ms;
		free(temp);
		print_message(data, print);
		return (1);
	}
	else
		pthread_mutex_unlock(&(data->m_print));
	return (0);
}

int	check_status(t_data *data)
{
	int	i;
	int	still_eating;

	i = 0;
	still_eating = data->philo_count;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&(data->p[i].m_status));
		if ((get_time_ms(&(data->p[i])) - data->p[i].time_last_meal)
			>= data->time_to_die && data->p[i].status != DEAD)
		{
			data->p[i].status = DEAD;
			add_print(data, &(data->p[i]), DIED, get_time_ms(&(data->p[i])));
		}
		if (data->p[i].status == ERROR || data->p[i].status == DEAD)
		{
			pthread_mutex_unlock(&(data->p[i].m_status));
			return (change_all_status(data, DONE));
		}
		if (data->p[i].meals_consumed == data->meal_count)
			still_eating--;
		pthread_mutex_unlock(&(data->p[i++].m_status));
	}
	return (still_eating);
}

void	*printer(void *vargp)
{
	t_data	*data;

	data = (t_data *) vargp;
	while (check_status(data) > 0)
		print_message_from_print_list(data);
	if (check_status(data) == 0)
	{
		change_all_status(data, DONE);
		while (print_message_from_print_list(data))
			;
	}
	if (check_status(data) == -1)
		free_print_list_philo_died(data);
	return (NULL);
}
