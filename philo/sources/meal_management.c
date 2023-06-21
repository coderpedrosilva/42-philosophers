#include "philo.h"

int	meal(t_philo *phil)
{
	int	print_result;
	int	pwait_result;

	if (pthread_mutex_lock(phil->finalmeal_lock))
		return (exec_print(phil, ERROR, "mutex lock error"));
	phil->final_meal = timenow();
	if (pthread_mutex_unlock(phil->finalmeal_lock))
		return (exec_print(phil, ERROR, "mutex unlock error"));
	print_result = exec_print(phil, PHILO, EAT);
	pwait_result = await_process(phil->data->time_to_eat,
			phil->data->time_to_die);
	if (print_result || pwait_result)
		return (1);
	else
		return (0);
}

t_fork	*select_fork(t_philo *phil)
{
	if (phil->id % 2)
		return (phil->left_fork);
	else
		return (phil->right_fork);
}

static int	drop_fork(t_philo *phil, t_fork *curr_fork)
{
	if (pthread_mutex_unlock(curr_fork->fork_lock))
		return (exec_print(phil, ERROR, "mutex unlock error"));
	if (phil->data->nbr == 1)
		return (1);
	else
		return (0);
}

int	forks(t_philo *phil, t_fork *fork)
{
	int		i;
	t_fork	*curr_fork;

	i = 3;
	curr_fork = fork;
	while (--i)
	{
		if (pthread_mutex_lock(curr_fork->fork_lock))
			return (exec_print(phil, ERROR, "mutex lock error"));
		if (phil->id == curr_fork->last_user)
		{
			if (drop_fork(phil, curr_fork))
				return (1);
			continue ;
		}
		curr_fork->last_user = phil->id;
		if (exec_print(phil, PHILO, FORK)
			|| (phil->data->nbr == 1 && drop_fork(phil, curr_fork)))
			return (1);
		if (phil->id % 2)
			curr_fork = phil->right_fork;
		else
			curr_fork = phil->left_fork;
	}
	return (0);
}
