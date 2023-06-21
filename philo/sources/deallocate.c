#include "philo.h"

static void	deallocate_mtx(pthread_mutex_t *mtx)
{
	int	res;

	if (mtx)
	{
		res = pthread_mutex_destroy(mtx);
		if (res && res != 22)
			return ;
	}
	free(mtx);
}

static void	deallocate(t_data *data)
{
	size_t	i;

	deallocate_mtx(data->print_lock);
	deallocate_mtx(data->finish_lock);
	i = 0;
	while (i < data->nbr)
	{
		if (data->philos)
			deallocate_mtx(data->philos[i].finalmeal_lock);
		if (data->forks)
			deallocate_mtx(data->forks[i].fork_lock);
		if (data->threads)
			free(data->threads[i]);
		i++;
	}
	free(data->philos);
	free(data->forks);
	free(data->threads);
}

int	error(char *mes, t_data *data)
{
	if (mes)
		printf("Error : %s\n", mes);
	if (data)
		deallocate(data);
	return (1);
}
