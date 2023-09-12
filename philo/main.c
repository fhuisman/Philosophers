/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhuisman <fhuisman@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 15:03:11 by fhuisman      #+#    #+#                 */
/*   Updated: 2023/06/29 12:29:10 by fhuisman      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_data	data;

	if (init(&data, argc, argv) != 0)
		return (-1);
	init_routine(&data);
	free_all(&data);
	return (0);
}
