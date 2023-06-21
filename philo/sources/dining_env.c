#include "philo.h"

static int	dining_env_mtx(pthread_mutex_t **mtx, t_data *data)
{
	int	result;

	*mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!*mtx)
		return (error("malloc error", data));
	result = pthread_mutex_init(*mtx, NULL);
	if (result != 0)
	{
		error("mutex error", data);
		return (result);
	}
	return (0);
}

static int	dining_env_sim(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->nbr)
	{
		if (dining_env_mtx(&data->forks[i].fork_lock, data)
			|| dining_env_mtx(&data->philos[i].finalmeal_lock, data))
			return (1);
		data->philos[i].active = 1;
		data->philos[i].data = data;
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = &(data->forks[i]);
		data->philos[i].right_fork = &(data->forks[(i + 1) % data->nbr]);
		data->philos[i].had_meal = 0;
		data->threads[i] = (pthread_t *)malloc(sizeof(pthread_t));
		if (!data->threads[i++])
			return (error("malloc error", data));
	}
	return (0);
}

int	dining_env(t_data *data)
{
	data->ongoing = data->nbr;
	data->philos = (t_philo *)malloc(sizeof(t_philo) * data->nbr);
	if (!data->philos)
		return (error("malloc error", data));
	memset(data->philos, 0, sizeof(t_fork) * data->nbr);
	data->forks = (t_fork *)malloc(sizeof(t_fork) * data->nbr);
	if (!data->forks)
		return (error("malloc error", data));
	memset(data->forks, 0, sizeof(t_fork) * data->nbr);
	data->threads = (pthread_t **)malloc(sizeof(pthread_t *) * data->nbr);
	if (!data->threads)
		return (error("malloc error", data));
	memset(data->threads, 0, sizeof(pthread_t *) * data->nbr);
	return (dining_env_mtx(&data->print_lock, data)
		|| dining_env_mtx(&data->finish_lock, data)
		|| dining_env_sim(data));
}
