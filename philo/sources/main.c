/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: progerio <progerio@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 20:25:47 by progerio          #+#    #+#             */
/*   Updated: 2023/06/21 20:25:50 by progerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	analyzer(t_data *data, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (error("invalid argument", NULL));
	if (argc == 5)
		data->overall_meals = -1;
	if (ft_atoi(&data->nbr, argv[1]) || !data->nbr
		|| ft_atoi(&data->time_to_die, argv[2])
		|| ft_atoi(&data->time_to_eat, argv[3])
		|| ft_atoi(&data->time_to_sleep, argv[4]))
		return (error("invalid argument", NULL));
	if (argc == 6)
	{
		if (ft_atoi(&data->overall_meals, argv[5]))
			return (error("invalid argument", NULL));
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	size_t	i;

	memset(&data, 0, sizeof(t_data));
	if (analyzer(&data, argc, argv))
		return (1);
	if (argc == 6 && !data.overall_meals)
		return (0);
	if (dining_env(&data))
		return (1);
	if (philo_dinner(&data))
	{
		i = 0;
		while (i < data.nbr)
		{
			if (pthread_join(*data.threads[i], NULL)
				&& error("thread error", NULL))
				break ;
			i++;
		}
	}
	error(NULL, &data);
	return (0);
}
