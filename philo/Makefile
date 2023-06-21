NAME	=	philo

CC		=	gcc
INCLUDE	=	includes
CFLAGS	=	-Wall -Wextra -Werror -I$(INCLUDE)
LDFLAGS =   -lpthread
RM		=	rm -f

SRCS	=	sources/dining_env.c \
			sources/execution.c \
			sources/main.c \
			sources/deallocate.c \
			sources/meal_management.c \
			sources/controller.c \
			sources/mealtime_utilities.c \

OBJS	=	$(SRCS:%.c=%.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
			@echo "$(GREEN)$(NAME) created$(DEFAULT)"

.c.o:
			@$(CC) $(CFLAGS) -c $< -o $(<:.c=.o)

clean:
			@$(RM) $(OBJS)
			@echo "$(YELLOW)object files deleted$(DEFAULT)"
			
fclean:		clean
			@$(RM) $(NAME)
			@echo "$(RED)all deleted$(DEFAULT)"

re:			fclean all

.PHONY:		all clean fclean re

GREEN = \033[1;32m
YELLOW = \033[1;33m
RED = \033[1;31m
DEFAULT = \033[0m
