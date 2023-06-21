/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: progerio <progerio@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 20:25:39 by progerio          #+#    #+#             */
/*   Updated: 2023/06/21 20:25:41 by progerio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	exec_print(t_philo *phil, int flag, char *mes)
{
	if (pthread_mutex_lock(phil->data->print_lock))
		return (error("mutex lock error", NULL));
	if (flag == ERROR)
		error(mes, NULL);
	else
	{
		if (pthread_mutex_lock(phil->data->finish_lock))
			return (error("mutex lock error", NULL));
		if (!phil->data->finish)
			printf("%zu %zu %s", timenow() - phil->data->init, phil->id, mes);
		if (pthread_mutex_unlock(phil->data->finish_lock))
			return (error("mutex unlock error", NULL));
	}
	if (pthread_mutex_unlock(phil->data->print_lock))
		return (error("mutex unlock error", NULL));
	if (flag == ERROR)
		return (1);
	else
		return (0);
}

static int	is_exec_finished(t_philo *phil, int after_eat)
{
	int	flag;
	int	unlock_result;

	if (pthread_mutex_lock(phil->data->finish_lock))
		return (exec_print(phil, ERROR, "mutex lock error"));
	flag = 0;
	if (phil->data->finish)
		flag = 1;
	if (!flag && after_eat && (++phil->had_meal == phil->data->overall_meals))
	{
		flag = 1;
		phil->data->ongoing--;
		phil->active = 0;
	}
	unlock_result = pthread_mutex_unlock(phil->data->finish_lock);
	if (unlock_result)
		return (exec_print(phil, ERROR, "mutex unlock error"));
	else
		return (flag);
}

int	await_process(size_t to_do, size_t to_die)
{
	size_t	finish;
	size_t	curr;
	size_t	begin;

	finish = timenow() + to_do;
	curr = timenow();
	begin = curr;
	while (curr < finish)
	{
		usleep(100);
		curr = timenow();
		if (curr - begin > to_die)
			return (1);
	}
	return (0);
}

static void	*process(void *p)
{
	t_philo	*phil;
	int		res;

	phil = (t_philo *)p;
	while (1)
	{
		if (is_exec_finished(phil, 0) || forks(phil, select_fork(phil)))
			return (NULL);
		res = is_exec_finished(phil, 0);
		if (!res && meal(phil))
			res = 1;
		if (pthread_mutex_unlock(phil->right_fork->fork_lock)
			|| pthread_mutex_unlock(phil->left_fork->fork_lock))
		{
			exec_print(phil, ERROR, "mutex unlock error");
			res = 1;
		}
		if (res || is_exec_finished(phil, 1) || exec_print(phil, PHILO, SLEEP)
			|| await_process(phil->data->time_to_sleep, phil->data->time_to_die)
			|| is_exec_finished(phil, 0) || exec_print(phil, PHILO, THINK))
			return (NULL);
	}
}

int	philo_dinner(t_data *data)
{
	size_t	i;

	i = 0;
	data->init = timenow();
	while (i < data->nbr)
	{
		data->philos[i].final_meal = data->init;
		if (pthread_create(data->threads[i], NULL, process, &data->philos[i]))
		{
			if (error("thread error", NULL))
				return (0);
		}
		i++;
	}
	if (controller(data))
		return (0);
	else
		return (1);
}
