#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

# define FORK "has taken a fork\n"
# define EAT "is eating\n"
# define THINK "is thinking\n"
# define SLEEP "is sleeping\n"
# define DIED "died\n"

typedef enum e_print
{
	ERROR,
	PHILO
}				t_print;

typedef struct s_data	t_data;

typedef struct s_fork
{
	pthread_mutex_t	*fork_lock;
	size_t			last_user;
}				t_fork;

typedef struct s_philo
{
	int				active;
	t_data			*data;
	size_t			id;
	size_t			had_meal;
	size_t			final_meal;
	t_fork			*left_fork;
	t_fork			*right_fork;
	pthread_mutex_t	*finalmeal_lock;
}				t_philo;

typedef struct s_data
{
	int				finish;
	t_philo			*philos;
	t_fork			*forks;
	size_t			nbr;
	size_t			init;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			time_to_die;
	size_t			overall_meals;
	size_t			ongoing;
	pthread_t		**threads;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*finish_lock;
}				t_data;

int		controller(t_data *data);
int		error(char *mes, t_data *data);
int		dining_env(t_data *data);
int		exec_print(t_philo *phil, int flag, char *mes);
int		await_process(size_t to_do, size_t to_die);
int		philo_dinner(t_data *data);
int		meal(t_philo *phil);
t_fork	*select_fork(t_philo *phil);
int		forks(t_philo *phil, t_fork *fork);
int		ft_atoi(size_t *target, char *arg);
size_t	timenow(void);

#endif
