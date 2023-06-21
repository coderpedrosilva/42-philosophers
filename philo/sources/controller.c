#include "philo.h"

static int	notify_death(t_philo *phil)
{
	int	lock_result;
	int	unlock_result;

	lock_result = pthread_mutex_lock(phil->data->print_lock);
	if (lock_result)
		return (error("mutex lock error", NULL));
	printf("%zu %zu died\n", timenow() - phil->data->init, phil->id);
	unlock_result = pthread_mutex_unlock(phil->data->print_lock);
	if (unlock_result)
		return (error("mutex unlock error", NULL));
	return (0);
}

static int	inspect_vitality(t_data *data, size_t id, int *susp, int *print)
{
	int	result;

	result = 0;
	if (pthread_mutex_lock(data->finish_lock))
		return (exec_print(&data->philos[0], ERROR, "mutex lock error"));
	if (!data->ongoing)
		*susp = 0;
	*print = 0;
	if (data->philos[id].active)
	{
		*print = 1;
		data->finish = 1;
	}
	if (pthread_mutex_unlock(data->finish_lock))
		return (exec_print(&data->philos[0], ERROR, "mutex unlock error"));
	if ((!*susp || *print) && pthread_mutex_unlock(data->philos[id]
			.finalmeal_lock))
		return (exec_print(&data->philos[0],
				ERROR, "mutex unlock error"));
	if (*print)
		result = notify_death(&data->philos[id]);
	return (result);
}

int	controller(t_data *data)
{
	size_t	id;
	int		susp;
	int		print;

	id = 0;
	while (1)
	{
		usleep(100);
		if (pthread_mutex_lock(data->philos[id].finalmeal_lock))
			return (exec_print(&data->philos[0], ERROR, "mutex lock error"));
		susp = 0;
		if (data->time_to_die < timenow() - data->philos[id].final_meal)
			susp = 1;
		if (susp)
		{
			if (inspect_vitality(data, id, &susp, &print))
				return (1);
			if (!susp || print)
				return (0);
		}
		if (pthread_mutex_unlock(data->philos[id].finalmeal_lock))
			return (exec_print(&data->philos[0], ERROR, "mutex unlock error"));
		id = (id + 1) % data->nbr;
	}
}
