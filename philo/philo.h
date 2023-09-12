/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 15:03:21 by fhuisman      #+#    #+#                 */
/*   Updated: 2023/06/13 13:31:22 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>

# define ERR_USAGE "Usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]"
# define ERR_MALLOC "Memory allocation failed"
# define ERR_CREATE_THREAD "Creating thread failed"
# define ERR_JOIN_THREAD "Joining thread failed"
# define ERR_MUTEX_INIT "Initiating mutex failed"
# define ERR_GET_TIME "Getting time of day failed"
# define MSG_EATING "is eating"
# define MSG_SLEEPING "is sleeping"
# define MSG_THINKING "is thinking"
# define MSG_DIED "died"
# define MSG_FORK "has taken a fork"

typedef struct s_data	t_data;
typedef struct s_philo	t_philo;
typedef struct s_print	t_print;

typedef enum status
{
	ALIVE,
	DONE,
	DEAD,
	ERROR,
}	t_status;

typedef enum msg
{
	EATING,
	SLEEPING,
	THINKING,
	DIED,
	FORK,
}	t_msg;

struct s_data
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_count;
	struct timeval	start;
	struct s_philo	*p;
	pthread_t		print_thread;
	struct s_print	*print;
	pthread_mutex_t	m_print;
};

struct s_philo
{
	int				philo_id;
	int				meals_consumed;
	int				time_last_meal;
	pthread_mutex_t	r_fork;
	pthread_mutex_t	*l_fork;
	struct s_data	*data;
	t_status		status;
	pthread_mutex_t	m_status;
	pthread_t		p_thread;
};

struct s_print
{
	int				philo_id;
	t_msg			msg;
	int				time_ms;
	struct s_print	*next;
};

int		write_error(char *str);
int		init(t_data *data, int argc, char *argv[]);
int		ft_atoi(char *s);
int		init_routine(t_data *data);
void	detach_threads(t_data *data, int from, int to);
void	*printer(void *vargp);
void	*routine(void *vargp);
void	destr_mut(t_data *data, int i);
void	add_print(t_data *data, t_philo *philo, t_msg msg, int time_ms);
int		get_time_ms(t_philo *philo);
void	wait_for(t_philo *philo, int ms);
void	philo_error(char *message, t_philo *philo);
int		check_status(t_data *data);
void	free_all(t_data *data);
int		change_all_status(t_data *data, enum status status);
int		print_message_from_print_list(t_data *data);
void	free_print_list_philo_died(t_data *data);

#endif